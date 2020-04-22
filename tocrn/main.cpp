#include "tocrn.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "crnlib.h"
#define STBI_NO_STDIO
#include "stb_image.h"

const int cDefaultCRNQualityLevel = 128;
const bool srgb_colorspace = true;
const  bool use_adaptive_block_sizes = true;
const bool enable_dxt1a = false;

static void* error(const char* pMsg) {
    return nullptr;
}

void* compress(const unsigned char *pSrcData,   unsigned int scrSize,  unsigned int *pSize) {
    *pSize = 0;

    int width, height, actual_comps;
    crn_uint32* pSrc_image = (crn_uint32*)stbi_load_from_memory(pSrcData, scrSize, &width, &height, &actual_comps, 4);
    if (!pSrc_image) {
      return error("Failed reading image file!\n");
    }
   
   bool has_alpha_channel = actual_comps > 3;

    crn_comp_params comp_params;
    comp_params.m_width = width;
    comp_params.m_height = height;
    comp_params.set_flag(cCRNCompFlagPerceptual, srgb_colorspace);
    comp_params.set_flag(cCRNCompFlagDXT1AForTransparency, enable_dxt1a && has_alpha_channel);
    comp_params.set_flag(cCRNCompFlagHierarchical, use_adaptive_block_sizes);
    comp_params.m_file_type =  cCRNFileTypeCRN;
    comp_params.m_format = has_alpha_channel ? cCRNFmtETC2A : cCRNFmtETC2;
    comp_params.m_pImages[0][0] = pSrc_image;
    comp_params.m_quality_level = cDefaultCRNQualityLevel;

    comp_params.m_num_helper_threads = 0;

    comp_params.m_pProgress_func = nullptr;

    crn_mipmap_params mip_params;
    mip_params.m_gamma_filtering = srgb_colorspace;
    mip_params.m_mode = cCRNMipModeNoMips;

    crn_uint32 actual_quality_level;
    float actual_bitrate;
    crn_uint32 output_file_size;

    void* pOutput_data = crn_compress(comp_params, mip_params, output_file_size, &actual_quality_level, &actual_bitrate);

    if (!pOutput_data) {
      stbi_image_free(pSrc_image);
      return error("Compression failed!");
    }

    stbi_image_free(pSrc_image);

    *pSize = output_file_size;

    return pOutput_data;
}

void freeCompressData(void* pData) {
    crn_free_block(pData);
}
