//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2021, Alex Fuller. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//
//      * Neither the name of Image Engine Design nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#include "GafferDenoise/ImageAlgo.h"
#include "GafferDenoise/DenoiseOIDN.h"

#include "GafferImage/BufferAlgo.h"
#include "GafferImage/ImageAlgo.h"

#include "IECore/MessageHandler.h"
#include "IECore/SimpleTypedData.h"
#include "IECore/VectorTypedData.h"

#include "OpenImageDenoise/oidn.hpp"

using namespace std;
using namespace Imath;
using namespace IECore;
using namespace Gaffer;
using namespace GafferImage;
using namespace GafferDenoise;

namespace
{

// Default layer names
IECore::InternedString g_rgbLayerName( "[RGB]" );
IECore::InternedString g_albedoLayerName( "albedo" );
IECore::InternedString g_normalLayerName( "normal" );
// Default filter name
IECore::InternedString g_RTFilterName( "RT" );
IECore::InternedString g_RTLightmapFilterName( "RTLightmap" );

}

IE_CORE_DEFINERUNTIMETYPED( DenoiseOIDN );

size_t DenoiseOIDN::g_firstPlugIndex = 0;

DenoiseOIDN::DenoiseOIDN( const std::string &name )
	:   FrameProcessor( name )
{
	storeIndexOfNextChild( g_firstPlugIndex );
	// Common
	addChild( new IntPlug( "verbose", Gaffer::Plug::In, 0 ) );
	addChild( new IntPlug( "numThreads", Gaffer::Plug::In, 0 ) );
	addChild( new BoolPlug( "setAffinity", Gaffer::Plug::In, true ) );
	addChild( new IntPlug( "deviceType", Gaffer::Plug::In, 0 ) ); // OIDN_DEVICE_TYPE_DEFAULT
	addChild( new StringPlug( "filterType", Gaffer::Plug::In, g_RTFilterName ) );

	// Filter-specific
	addChild( new StringPlug( "albedo", Gaffer::Plug::In, g_albedoLayerName ) );
	addChild( new StringPlug( "normal", Gaffer::Plug::In, g_normalLayerName ) );

	addChild( new BoolPlug( "hdr", Gaffer::Plug::In, true ) );
	addChild( new FloatPlug( "inputScale", Gaffer::Plug::In, 0.0f ) );
	addChild( new BoolPlug( "srgb", Gaffer::Plug::In, false ) );
	addChild( new IntPlug( "maxMemoryMB", Gaffer::Plug::In, 3000 ) );
	addChild( new BoolPlug( "cleanAux", Gaffer::Plug::In, false ) );
}

DenoiseOIDN::~DenoiseOIDN()
{
}

Gaffer::IntPlug *DenoiseOIDN::verbosePlug()
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

const Gaffer::IntPlug *DenoiseOIDN::verbosePlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex );
}

Gaffer::IntPlug *DenoiseOIDN::numThreadsPlug()
{
	return getChild<IntPlug>( g_firstPlugIndex + 1 );
}

const Gaffer::IntPlug *DenoiseOIDN::numThreadsPlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex + 1 );
}

Gaffer::BoolPlug *DenoiseOIDN::setAffinityPlug()
{
	return getChild<BoolPlug>( g_firstPlugIndex + 2 );
}

const Gaffer::BoolPlug *DenoiseOIDN::setAffinityPlug() const
{
	return getChild<BoolPlug>( g_firstPlugIndex + 2 );
}

Gaffer::IntPlug *DenoiseOIDN::deviceTypePlug()
{
	return getChild<IntPlug>( g_firstPlugIndex + 3 );
}

const Gaffer::IntPlug *DenoiseOIDN::deviceTypePlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex + 3 );
}

Gaffer::StringPlug *DenoiseOIDN::filterTypePlug()
{
	return getChild<StringPlug>( g_firstPlugIndex + 4 );
}

const Gaffer::StringPlug *DenoiseOIDN::filterTypePlug() const
{
	return getChild<StringPlug>( g_firstPlugIndex + 4 );
}

Gaffer::StringPlug *DenoiseOIDN::albedoPlug()
{
	return getChild<StringPlug>( g_firstPlugIndex + 5 );
}

const Gaffer::StringPlug *DenoiseOIDN::albedoPlug() const
{
	return getChild<StringPlug>( g_firstPlugIndex + 5 );
}

Gaffer::StringPlug *DenoiseOIDN::normalPlug()
{
	return getChild<StringPlug>( g_firstPlugIndex + 6 );
}

const Gaffer::StringPlug *DenoiseOIDN::normalPlug() const
{
	return getChild<StringPlug>( g_firstPlugIndex + 6 );
}

Gaffer::BoolPlug *DenoiseOIDN::hdrPlug()
{
	return getChild<BoolPlug>( g_firstPlugIndex + 7 );
}

const Gaffer::BoolPlug *DenoiseOIDN::hdrPlug() const
{
	return getChild<BoolPlug>( g_firstPlugIndex + 7 );
}

Gaffer::FloatPlug *DenoiseOIDN::inputScalePlug()
{
	return getChild<FloatPlug>( g_firstPlugIndex + 8 );
}

const Gaffer::FloatPlug *DenoiseOIDN::inputScalePlug() const
{
	return getChild<FloatPlug>( g_firstPlugIndex + 8 );
}

Gaffer::BoolPlug *DenoiseOIDN::srgbPlug()
{
	return getChild<BoolPlug>( g_firstPlugIndex + 9 );
}

const Gaffer::BoolPlug *DenoiseOIDN::srgbPlug() const
{
	return getChild<BoolPlug>( g_firstPlugIndex + 9 );
}

Gaffer::IntPlug *DenoiseOIDN::maxMemoryMBPlug()
{
	return getChild<IntPlug>( g_firstPlugIndex + 10 );
}

const Gaffer::IntPlug *DenoiseOIDN::maxMemoryMBPlug() const
{
	return getChild<IntPlug>( g_firstPlugIndex + 10 );
}

Gaffer::BoolPlug *DenoiseOIDN::cleanAuxPlug()
{
	return getChild<BoolPlug>( g_firstPlugIndex + 11 );
}

const Gaffer::BoolPlug *DenoiseOIDN::cleanAuxPlug() const
{
	return getChild<BoolPlug>( g_firstPlugIndex + 11 );
}

bool DenoiseOIDN::affectsColorData( const Gaffer::Plug *input ) const
{
	if( FrameProcessor::affectsColorData( input ) )
	{
		return true;
	}
	if( 
		input == inPlug()->channelDataPlug() ||
		input == inPlug()->channelNamesPlug() ||
		input == deviceTypePlug() ||
		input == filterTypePlug() ||
		input == albedoPlug() ||
		input == normalPlug() ||
		input == hdrPlug() ||
		input == inputScalePlug() ||
		input == srgbPlug() ||
		input == cleanAuxPlug()
	)
	{
		return true;
	}
}

void DenoiseOIDN::hashColorData( const Gaffer::Context *context, IECore::MurmurHash &h ) const
{
	FrameProcessor::hashColorData( context, h );

	ImagePlug::GlobalScope c( context );
	deviceTypePlug()->hash( h );
	filterTypePlug()->hash( h );

	albedoPlug()->hash( h );
	normalPlug()->hash( h );

	hdrPlug()->hash( h );
	inputScalePlug()->hash( h );
	srgbPlug()->hash( h );
	cleanAuxPlug()->hash( h );

}

void DenoiseOIDN::processColorData( const Gaffer::Context *context, IECore::FloatVectorData *r, IECore::FloatVectorData *g, IECore::FloatVectorData *b ) const
{
	int width = inPlug()->format().width();
	int height = inPlug()->format().height();
	IECore::Color3fVectorDataPtr colorData = new IECore::Color3fVectorData();
	std::vector<Imath::Color3f> &d = colorData->writable();
	d.resize( width * height );

	if( ImageAlgo::interleave( r, g, b, colorData.get() ) )
	{
		oidn::DeviceRef device = oidn::newDevice( static_cast<oidn::DeviceType>( deviceTypePlug()->getValue() ) );
		if( device )
		{
			device.commit();
		}
		else
		{
			return;
		}

		InternedString filterType( filterTypePlug()->getValue() );

		ConstStringVectorDataPtr channelNamesData;
		{
			GafferImage::ImagePlug::GlobalScope globalScope( context );
			channelNamesData = inPlug()->channelNamesPlug()->getValue();
		}
		const vector<string> &channelNames = channelNamesData->readable();

		bool hasAlbedo = false;
		IECore::FloatVectorDataPtr albedoIn[3];
		IECore::Color3fVectorDataPtr albedoData = new IECore::Color3fVectorData();

		bool hasNormal = false;
		IECore::FloatVectorDataPtr normalIn[3];
		IECore::Color3fVectorDataPtr normalData = new IECore::Color3fVectorData();

		//GafferImage::ImagePlug::ChannelDataScope channelDataScope( context );
		if( filterType == g_RTFilterName )
		{
			GafferImage::ImagePlug::GlobalScope globalScope( context );
			IECoreImage::ImagePrimitivePtr image = GafferImage::ImageAlgo::image( inPlug() );

			int i = 0;
			for( const auto &baseName : { "R", "G", "B" } )
			{
				string channelName = GafferImage::ImageAlgo::channelName( albedoPlug()->getValue(), baseName );
				if( GafferImage::ImageAlgo::channelExists( channelNames, channelName ) )
				{
					albedoIn[i] = image->getChannel<float>( channelName );
				}
				i++;
			}

			if( albedoIn[0] && albedoIn[1] && albedoIn[2] )
			{
				std::vector<Imath::Color3f> &ad = albedoData->writable();
				ad.resize( width * height );
				hasAlbedo = ImageAlgo::interleave( albedoIn[0].get(), albedoIn[1].get(), albedoIn[2].get(), albedoData.get() );
			}

			i = 0;
			for( const auto &baseName : { "R", "G", "B" } )
			{
				string channelName = GafferImage::ImageAlgo::channelName( normalPlug()->getValue(), baseName );
				if( GafferImage::ImageAlgo::channelExists( channelNames, channelName ) )
				{
					normalIn[i] = image->getChannel<float>( channelName );
				}
				i++;
			}

			if( normalIn[0] && normalIn[1] && normalIn[2] )
			{
				std::vector<Imath::Color3f> &nd = normalData->writable();
				nd.resize( width * height );
				hasNormal = ImageAlgo::interleave( normalIn[0].get(), normalIn[1].get(), normalIn[2].get(), normalData.get() );
			}
		}

		IECore::Color3fVectorDataPtr colorIn = colorData->copy();
		std::vector<Imath::Color3f> &color = colorIn->writable(); // readable, but oidn wants a non-const ptr
		std::vector<Imath::Color3f> &albedo = albedoData->writable(); // readable, but oidn wants a non-const ptr
		std::vector<Imath::Color3f> &normal = normalData->writable(); // readable, but oidn wants a non-const ptr
		std::vector<Imath::Color3f> &output = colorData->writable();

		oidn::FilterRef filter = device.newFilter( filterTypePlug()->getValue().c_str() );

		filter.setImage( "color", &color[0], oidn::Format::Float3, width, height );

		if( hasAlbedo )
			filter.setImage( "albedo", &albedo[0], oidn::Format::Float3, width, height );

		if( hasNormal )
			filter.setImage( "normal", &normal[0], oidn::Format::Float3, width, height );

		filter.setImage( "output", &output[0], oidn::Format::Float3, width, height );

		if( filterType == g_RTFilterName )
		{
			filter.set( "hdr", hdrPlug()->getValue() );
			filter.set( "srgb", srgbPlug()->getValue() );
		}

		// OIDN 1.1.0
		float inputScale = inputScalePlug()->getValue();
		if( inputScale > 0.0f )
		{
			filter.set( "inputScale", inputScale );
		}
		filter.set( "maxMemoryMB", maxMemoryMBPlug()->getValue() );

		filter.set( "cleanAux", cleanAuxPlug()->getValue() );

		filter.commit();
		filter.execute();

		const char* errorMessage;
		if( device.getError( errorMessage ) != oidn::Error::None )
		{
			IECore::msg( IECore::Msg::Error, "GafferOIDN::Denoise", boost::format( "%s" ) % errorMessage );
		}

		ImageAlgo::deinterleave( r, g, b, colorData.get() );
	}
}