import GafferUI

import GafferDenoise
import GafferDenoiseUI

nodeMenu = GafferUI.NodeMenu.acquire( application )
nodeMenu.append( "/Denoise/OIDN", GafferDenoiseUI.DenoiseOIDNUI.nodeMenuCreateCommand, searchText = "DenoiseOIDN" )
#nodeMenu.append( "/Denoise/Optix", GafferDenoiseUI.DenoiseOptixUI.nodeMenuCreateCommand, searchText = "DenoiseOptix" )
