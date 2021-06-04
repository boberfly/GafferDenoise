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
//      * Neither the name of Alex Fuller nor the names of
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

#ifndef GAFFERDENOISE_DENOISEOIDN_H
#define GAFFERDENOISE_DENOISEOIDN_H

#include "GafferDenoise/Export.h"
#include "GafferDenoise/TypeIds.h"

#include "Gaffer/PlugType.h"

#include "GafferDenoise/FrameProcessor.h"

namespace GafferDenoise
{

IE_CORE_FORWARDDECLARE( DenoiseOIDN )

class GAFFERDENOISE_API DenoiseOIDN : public FrameProcessor
{
	public :

		DenoiseOIDN( const std::string &name=defaultName<DenoiseOIDN>() );
		~DenoiseOIDN() override;

		GAFFER_GRAPHCOMPONENT_DECLARE_TYPE( GafferDenoise::DenoiseOIDN, GafferDenoise::DenoiseOIDNTypeId, FrameProcessor );

		Gaffer::IntPlug *verbosePlug();
		const Gaffer::IntPlug *verbosePlug() const;

		Gaffer::IntPlug *numThreadsPlug();
		const Gaffer::IntPlug *numThreadsPlug() const;

		Gaffer::BoolPlug *setAffinityPlug();
		const Gaffer::BoolPlug *setAffinityPlug() const;

		Gaffer::IntPlug *deviceTypePlug();
		const Gaffer::IntPlug *deviceTypePlug() const;
		
		Gaffer::StringPlug *filterTypePlug();
		const Gaffer::StringPlug *filterTypePlug() const;

		Gaffer::StringPlug *albedoPlug();
		const Gaffer::StringPlug *albedoPlug() const;

		Gaffer::StringPlug *normalPlug();
		const Gaffer::StringPlug *normalPlug() const;

		Gaffer::BoolPlug *hdrPlug();
		const Gaffer::BoolPlug *hdrPlug() const;

		Gaffer::FloatPlug *inputScalePlug();
		const Gaffer::FloatPlug *inputScalePlug() const;

		Gaffer::BoolPlug *srgbPlug();
		const Gaffer::BoolPlug *srgbPlug() const;

		Gaffer::IntPlug *maxMemoryMBPlug();
		const Gaffer::IntPlug *maxMemoryMBPlug() const;

		Gaffer::BoolPlug *cleanAuxPlug();
		const Gaffer::BoolPlug *cleanAuxPlug() const;

	protected :

		bool affectsColorData( const Gaffer::Plug *input ) const override;
		void hashColorData( const Gaffer::Context *context, IECore::MurmurHash &h ) const override;
		void processColorData( const Gaffer::Context *context, IECore::FloatVectorData *r, IECore::FloatVectorData *g, IECore::FloatVectorData *b ) const override;

	private :

		static size_t g_firstPlugIndex;

};

IE_CORE_DECLAREPTR( DenoiseOIDN )

} // namespace GafferDenoise

#endif // GAFFERDENOISE_DENOISEOIDN_H
