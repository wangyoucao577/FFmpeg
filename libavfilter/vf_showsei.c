/*
 * Copyright (c) 2023 Jay Zhang <wangyoucao577@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * filter for showing SEI information
 */

#include "libavutil/opt.h"
#include "libavutil/log.h"

#include "avfilter.h"
#include "internal.h"

typedef struct ShowSEIContext {
    const AVClass *class;
} ShowSEIContext;

static const AVOption showsei_options[] = {};

AVFILTER_DEFINE_CLASS(showsei);


static int filter_frame(AVFilterLink *inlink, AVFrame *frame)
{
    if (!frame || frame->nb_side_data <=0 ){
        return 0;   // no SEI information
    }

    AVFilterContext *ctx = inlink->dst;


    //TODO: 

    av_log(ctx, AV_LOG_INFO, "frame pts %"PRId64" nb_side_data %d\n", frame->pts, frame->nb_side_data);
    return 0;
}

static int config_props(AVFilterContext *ctx, AVFilterLink *link, int is_out)
{

    av_log(ctx, AV_LOG_INFO, "config %s time_base: %d/%d, frame_rate: %d/%d\n",
           is_out ? "out" : "in",
           link->time_base.num, link->time_base.den,
           link->frame_rate.num, link->frame_rate.den);

    return 0;
}

static int config_props_in(AVFilterLink *link)
{
    AVFilterContext *ctx = link->dst;
    return config_props(ctx, link, 0);
}

static int config_props_out(AVFilterLink *link)
{
    AVFilterContext *ctx = link->src;
    return config_props(ctx, link, 1);
}

static const AVFilterPad avfilter_vf_showsei_inputs[] = {
    {
        .name             = "default",
        .type             = AVMEDIA_TYPE_VIDEO,
        .filter_frame     = filter_frame,
        .config_props     = config_props_in,
    },
};

static const AVFilterPad avfilter_vf_showsei_outputs[] = {
    {
        .name = "default",
        .type = AVMEDIA_TYPE_VIDEO,
        .config_props  = config_props_out,
    },
};

const AVFilter ff_vf_showsei = {
    .name        = "showsei",
    .description = NULL_IF_CONFIG_SMALL("Show SEI information extracted from video elementary stream."),
    FILTER_INPUTS(avfilter_vf_showsei_inputs),
    FILTER_OUTPUTS(avfilter_vf_showsei_outputs),
    .priv_size   = sizeof(ShowSEIContext),
    .priv_class  = &showsei_class,
    .flags       = AVFILTER_FLAG_METADATA_ONLY,
};
