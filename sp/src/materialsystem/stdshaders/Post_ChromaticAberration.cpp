// ----------------------------------------------------------------------------
// POSTPROC_LENSDISTORTION.CPP
//
// This file defines the C++ component of lens distortion post processing shader.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "postproc_vs30.inc"
#include "post_chromatic_aberration_ps30.inc"

BEGIN_SHADER( post_chromatic_aberration, "Lens distortion effect" )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( AMOUNT, SHADER_PARAM_TYPE_VEC2, "[-4 0.3]", "Distortion amount" )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if( !params[AMOUNT]->IsDefined() )
            params[AMOUNT]->SetVecValue(-4.0f, 0.3f);
    }

    SHADER_FALLBACK
    {
        // Requires DX9 + above
        if( g_pHardwareConfig->GetDXSupportLevel() < 90 ) {
            Assert( 0 );
            return "Wireframe";
        }
        return 0;
    }

    SHADER_DRAW
    {
        SHADOW_STATE
        {
            pShaderShadow->EnableDepthWrites( false );
            pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

            pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, 0, 0 );

            DECLARE_STATIC_VERTEX_SHADER( postproc_vs30 );
            SET_STATIC_VERTEX_SHADER( postproc_vs30 );

			DECLARE_STATIC_PIXEL_SHADER(post_chromatic_aberration_ps30);
			SET_STATIC_PIXEL_SHADER(post_chromatic_aberration_ps30);
        }

        DYNAMIC_STATE
        {
            pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0 );
            
            const float *CX = params[AMOUNT]->GetVecValue();
            pShaderAPI->SetPixelShaderConstant( 0, CX, 2 );

            DECLARE_DYNAMIC_VERTEX_SHADER( postproc_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( postproc_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER(post_chromatic_aberration_ps30);
			SET_DYNAMIC_PIXEL_SHADER(post_chromatic_aberration_ps30);
        }

        Draw();
    }

END_SHADER