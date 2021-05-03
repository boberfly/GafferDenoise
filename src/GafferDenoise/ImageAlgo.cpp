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

#include "GafferDenoise/ImageAlgo.h"

namespace GafferDenoise
{

namespace ImageAlgo
{

const std::string channelNameA( "A" );

bool interleave( 
	const IECore::FloatVectorData *rd, 
	const IECore::FloatVectorData *gd, 
	const IECore::FloatVectorData *bd,
	IECore::Color3fVectorData *rgb 
	)
{
	if( !(rd && gd && bd && rgb) )
	{
		return false;
	}

	const std::vector<float> &r = rd->readable();
	const std::vector<float> &g = gd->readable();
	const std::vector<float> &b = bd->readable();

	std::vector<Imath::Color3f> &d = rgb->writable();

    if( r.size() != d.size() )
    {
        return false;
    }

    for( int i = 0; i != r.size(); i++ )
    {
        d[i] = Imath::Color3f(r[i], g[i], b[i]);
    }

	return true;
}

bool deinterleave( 
	IECore::FloatVectorData *rd, 
	IECore::FloatVectorData *gd, 
	IECore::FloatVectorData *bd,
	const IECore::Color3fVectorData *rgb 
	)
{
	if( !(rd && gd && bd && rgb) )
	{
		return false;
	}

	std::vector<float> &r = rd->writable();
	std::vector<float> &g = gd->writable();
	std::vector<float> &b = bd->writable();

	const std::vector<Imath::Color3f> &d = rgb->readable();

    if( r.size() != d.size() )
    {
        return false;
    }

    for( int i = 0; i != r.size(); i++ )
    {
        Imath::Color3f color = d[i];
        r[i] = color[0];
        g[i] = color[1];
        b[i] = color[2];
    }

	return true;
}

} // namespace ImageAlgo

} // namespace GafferDenoise
