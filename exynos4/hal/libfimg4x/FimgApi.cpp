/*
**
** Copyright 2009 Samsung Electronics Co, Ltd.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
**
*/

#define LOG_NDEBUG 0
#define LOG_TAG "SKIA"
#include <utils/Log.h>

#include "FimgApi.h"

struct blit_op_table optbl[] = {
    { (int)BLIT_OP_SOLID_FILL, "FILL" },
    { (int)BLIT_OP_CLR, "CLR" },
    { (int)BLIT_OP_SRC, "SRC" },
    { (int)BLIT_OP_DST, "DST" },
    { (int)BLIT_OP_SRC_OVER, "SRC_OVER" },
    { (int)BLIT_OP_DST_OVER, "DST_OVER" },
    { (int)BLIT_OP_SRC_IN, "SRC_IN" },
    { (int)BLIT_OP_DST_IN, "DST_IN" },
    { (int)BLIT_OP_SRC_OUT, "SRC_OUT" },
    { (int)BLIT_OP_DST_OUT, "DST_OUT" },
    { (int)BLIT_OP_SRC_ATOP, "SRC_ATOP" },
    { (int)BLIT_OP_DST_ATOP, "DST_ATOP" },
    { (int)BLIT_OP_XOR, "XOR" },
    { (int)BLIT_OP_ADD, "ADD" },
    { (int)BLIT_OP_MULTIPLY, "MULTIPLY" },
    { (int)BLIT_OP_SCREEN, "SCREEN" },
    { (int)BLIT_OP_DARKEN, "DARKEN" },
    { (int)BLIT_OP_LIGHTEN, "LIGHTEN" },
    { (int)BLIT_OP_DISJ_SRC_OVER, "DISJ_SRC_OVER" },
    { (int)BLIT_OP_DISJ_DST_OVER, "DISJ_DST_OVER" },
    { (int)BLIT_OP_DISJ_SRC_IN, "DISJ_SRC_IN" },
    { (int)BLIT_OP_DISJ_DST_IN, "DISJ_DST_IN" },
    { (int)BLIT_OP_DISJ_SRC_OUT, "DISJ_SRC_OUT" },
    { (int)BLIT_OP_DISJ_DST_OUT, "DISJ_DST_OUT" },
    { (int)BLIT_OP_DISJ_SRC_ATOP, "DISJ_SRC_ATOP" },
    { (int)BLIT_OP_DISJ_DST_ATOP, "DISJ_DST_ATOP" },
    { (int)BLIT_OP_DISJ_XOR, "DISJ_XOR" },
    { (int)BLIT_OP_CONJ_SRC_OVER, "CONJ_SRC_OVER" },
    { (int)BLIT_OP_CONJ_DST_OVER, "CONJ_DST_OVER" },
    { (int)BLIT_OP_CONJ_SRC_IN, "CONJ_SRC_IN" },
    { (int)BLIT_OP_CONJ_DST_IN, "CONJ_DST_IN" },
    { (int)BLIT_OP_CONJ_SRC_OUT, "CONJ_SRC_OUT" },
    { (int)BLIT_OP_CONJ_DST_OUT, "CONJ_DST_OUT" },
    { (int)BLIT_OP_CONJ_SRC_ATOP, "CONJ_SRC_ATOP" },
    { (int)BLIT_OP_CONJ_DST_ATOP, "CONJ_DST_ATOP" },
    { (int)BLIT_OP_CONJ_XOR, "CONJ_XOR" },
    { (int)BLIT_OP_USER_COEFF, "USER_COEFF" },
    { (int)BLIT_OP_END, "" },
};

#ifndef REAL_DEBUG
    void VOID_FUNC(const char *format, ...)
    {}
#endif

FimgApi::FimgApi()
{
    m_flagCreate = false;
}

FimgApi::~FimgApi()
{
    if (m_flagCreate == true)
        PRINT("%s::this is not Destroyed fail\n", __func__);
}

bool FimgApi::Create(void)
{
    bool ret = false;

    if (t_Lock() == false) {
        PRINT("%s::t_Lock() fail\n", __func__);
        goto CREATE_DONE;
    }

    if (m_flagCreate == true) {
        PRINT("%s::Already Created fail\n", __func__);
        goto CREATE_DONE;
    }

    if (t_Create() == false) {
        PRINT("%s::t_Create() fail\n", __func__);
        goto CREATE_DONE;
    }

    m_flagCreate = true;

    ret = true;

CREATE_DONE :

    t_UnLock();

    return ret;
}

bool FimgApi::Destroy(void)
{
    bool ret = false;

    if (t_Lock() == false) {
        PRINT("%s::t_Lock() fail\n", __func__);
        goto DESTROY_DONE;
    }

    if (m_flagCreate == false) {
        PRINT("%s::Already Destroyed fail\n", __func__);
        goto DESTROY_DONE;
    }

    if (t_Destroy() == false) {
        PRINT("%s::t_Destroy() fail\n", __func__);
        goto DESTROY_DONE;
    }

    m_flagCreate = false;

    ret = true;

DESTROY_DONE :

    t_UnLock();

    return ret;
}

bool FimgApi::Stretch(struct fimg2d_blit *cmd)
{
    bool ret = false;

    if (t_Lock() == false) {
        PRINT("%s::t_Lock() fail\n", __func__);
        goto STRETCH_DONE;
    }

    if (m_flagCreate == false) {
        PRINT("%s::This is not Created fail\n", __func__);
        goto STRETCH_DONE;
    }

    if (t_Stretch(cmd) == false) {
        goto STRETCH_DONE;
    }

    ret = true;

STRETCH_DONE :

    t_UnLock();

    return ret;
}

bool FimgApi::Sync(void)
{
    bool ret = false;

    if (m_flagCreate == false) {
        PRINT("%s::This is not Created fail\n", __func__);
        goto SYNC_DONE;
    }

    if (t_Sync() == false)
        goto SYNC_DONE;

    ret = true;

SYNC_DONE :

    return ret;
}

bool FimgApi::t_Create(void)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

bool FimgApi::t_Destroy(void)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

bool FimgApi::t_Stretch(struct fimg2d_blit *cmd)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

bool FimgApi::t_Sync(void)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

bool FimgApi::t_Lock(void)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

bool FimgApi::t_UnLock(void)
{
    PRINT("%s::This is empty virtual function fail\n", __func__);
    return false;
}

//---------------------------------------------------------------------------//
// extern function
//---------------------------------------------------------------------------//
extern "C" int stretchFimgApi(struct fimg2d_blit *cmd)
{
    FimgApi * fimgApi = createFimgApi();

    if (fimgApi == NULL) {
        PRINT("%s::createFimgApi() fail\n", __func__);
        return -1;
    }

    if (fimgApi->Stretch(cmd) == false) {
        if (fimgApi != NULL)
            destroyFimgApi(fimgApi);

        return -1;
    }

    if (fimgApi != NULL)
        destroyFimgApi(fimgApi);

    return 0;
}

extern "C" int SyncFimgApi(void)
{
    FimgApi * fimgApi = createFimgApi();
    if (fimgApi == NULL) {
        PRINT("%s::createFimgApi() fail\n", __func__);
        return -1;
    }

    if (fimgApi->Sync() == false) {
        if (fimgApi != NULL)
            destroyFimgApi(fimgApi);

        return -1;
    }

    if (fimgApi != NULL)
        destroyFimgApi(fimgApi);

    return 0;
}

void printDataBlit(char *title, struct fimg2d_blit *cmd)
{
    LOGI("%s\n", title);

    LOGI("    sequence_no. = %u\n", cmd->seq_no);
    LOGI("    blit_op      = %d(%s)\n", cmd->op, optbl[cmd->op].str);
    LOGI("    fill_color   = %X\n", cmd->solid_color);
    LOGI("    global_alpha = %u\n", (unsigned int)cmd->g_alpha);
    LOGI("    PREMULT      = %s\n", cmd->premult == PREMULTIPLIED ? "PREMULTIPLIED" : "NON-PREMULTIPLIED");
    LOGI("    do_dither    = %s\n", cmd->dither == true ? "dither" : "no-dither");

    printDataBlitRotate(cmd->rotate);

    printDataBlitScale(cmd->scaling);

    printDataBlitImage("SRC", cmd->src);
    printDataBlitImage("DST", cmd->dst);
    printDataBlitImage("MSK", cmd->msk);

    printDataBlitRect("SRC", cmd->src_rect);
    printDataBlitRect("DST", cmd->dst_rect);
    printDataBlitRect("MSK", cmd->msk_rect);

    printDataBlitClip(cmd->clipping);
}

void printDataBlitImage(char *title, struct fimg2d_image *image)
{
    if (NULL != image) {
    LOGI("    Image_%s\n", title);
    LOGI("        addr = %X\n", image->addr.start);
    LOGI("        size = %u\n", image->addr.size);
    LOGI("        (width, height) = (%d, %d)\n", image->width, image->height);
    LOGI("        format = %d\n", image->fmt);
    } else
        LOGI("    Image_%s : NULL\n", title);
}

void printDataBlitRect(char *title, struct fimg2d_rect *rect)
{
    if (NULL != rect) {
        LOGI("    RECT_%s\n", title);
        LOGI("        (x1, y1) = (%d, %d)\n", rect->x1, rect->y1);
        LOGI("        (x2, y2) = (%d, %d)\n", rect->x2, rect->y2);
        LOGI("        (width, height) = (%d, %d)\n", rect->x2 - rect->x1, rect->y2 - rect->y1);
    } else
        LOGI("    RECT_%s : NULL\n", title);
}

void printDataBlitRotate(enum rotation rotate)
{
    LOGI("    ROTATE : %d\n", rotate);
}

void printDataBlitClip(struct fimg2d_clip *clip)
{
    if (NULL != clip) {
        LOGI("    CLIP\n");
        LOGI("        clip %s\n", clip->enable == true ? "enabled" : "disabled");
        LOGI("        (x1, y1) = (%d, %d)\n", clip->x1, clip->y1);
        LOGI("        (x2, y2) = (%d, %d)\n", clip->x2, clip->y2);
        LOGI("        (width, hight) = (%d, %d)\n", clip->x2 - clip->x1, clip->y2 - clip->y1);
    } else
        LOGI("    CLIP : NULL\n");
}

void printDataBlitScale(struct fimg2d_scale *scaling)
{
    if (NULL != scaling) {
        LOGI("    SCALING\n");
        LOGI("        scale_mode : %s\n", scaling->mode == 0 ?
                                          "NO_SCALING" :
                              (scaling->mode == 1 ? "SCALING_NEAREST" : "SCALING_BILINEAR"));
        LOGI("        scaling_factor_unit : %s\n", scaling->factor == 0 ? "PERCENT" : "PIXEL");

        if (scaling->factor == 0)
            LOGI("        scaling_factor : (scale_w, scale_y) = (%d, %d)\n", scaling->scale_w, scaling->scale_h);
        else {
            LOGI("        src : (src_w, src_h) = (%d, %d)\n", scaling->src_w, scaling->src_h);
            LOGI("        dst : (dst_w, dst_h) = (%d, %d)\n", scaling->dst_w, scaling->dst_h);
            LOGI("        scaling_factor : (scale_w, scale_y) = (%3.2f, %3.2f)\n", (double)scaling->dst_w / scaling->src_w, (double)scaling->dst_h / scaling->src_h);
        }
    } else
        LOGI("    SCALING : NULL(NO SCALE MODE)\n");

}

void printDataMatrix(int matrixType)
{
    LOGI("    MATRIX\n");

    if (matrixType & SkMatrix::kIdentity_Mask)
        LOGI("        Matrix_type : Identity_Mask\n");

    if (matrixType & SkMatrix::kTranslate_Mask)
        LOGI("        Matrix_type : Translate_Mask(the matrix has translation)\n");

    if (matrixType & SkMatrix::kScale_Mask)
        LOGI("        Matrix_type : Scale_Mask(the matrix has X or Y scale)\n");

    if (matrixType & SkMatrix::kAffine_Mask)
        LOGI("        Matrix_type : Affine_Mask(the matrix skews or rotates)\n");

    if (matrixType & SkMatrix::kPerspective_Mask)
        LOGI("        Matrix_type : Perspective_Mask(the matrix is in perspective)\n");
}
