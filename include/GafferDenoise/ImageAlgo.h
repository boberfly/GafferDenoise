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
//      * Neither the name of John Haddon nor the names of
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

#ifndef GAFFERDENOISE_IMAGEALGO_H
#define GAFFERDENOISE_IMAGEALGO_H

#include "GafferDenoise/Export.h"
#include "IECore/SimpleTypedData.h"
#include "IECore/VectorTypedData.h"

namespace GafferDenoise
{

namespace ImageAlgo
{

GAFFERDENOISE_API extern const std::string channelNameA;

GAFFERDENOISE_API bool interleave( 
	const IECore::FloatVectorData *rd, 
	const IECore::FloatVectorData *gd, 
	const IECore::FloatVectorData *bd,
	IECore::Color3fVectorData *rgb 
	);

GAFFERDENOISE_API bool deinterleave( 
	IECore::FloatVectorData *rd, 
	IECore::FloatVectorData *gd, 
	IECore::FloatVectorData *bd,
	const IECore::Color3fVectorData *rgb 
	);

} // namespace ImageAlgo

} // namespace GafferDenoise

#endif // GAFFERDENOISE_IMAGEALGO_H