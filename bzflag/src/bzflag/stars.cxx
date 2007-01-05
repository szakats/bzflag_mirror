/* bzflag
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "stars.h"

const float	stars[][6] = {
  { 1.00f, 1.00f, 1.00f, -0.18746032f, 0.93921369f, -0.28763914f },
  { 1.00f, 1.00f, 1.00f, -0.06323564f, 0.60291260f, -0.79529721f },
  { 1.00f, 1.00f, 1.00f, -0.78377002f, -0.52700269f, 0.32859191f },
  { 1.00f, 1.00f, 1.00f, 0.18718566f, 0.73014212f, 0.65715599f },
  { 0.99f, 0.99f, 0.99f, 0.12507832f, -0.76942003f, 0.62637711f },
  { 0.99f, 0.99f, 0.99f, 0.13051330f, 0.68228769f, 0.71933979f },
  { 0.98f, 0.98f, 0.98f, 0.19507207f, 0.97036278f, -0.14262892f },
  { 0.94f, 0.94f, 0.94f, -0.37387931f, -0.31261155f, -0.87320572f },
  { 0.94f, 0.94f, 0.94f, -0.41809806f, 0.90381104f, 0.09121194f },
  { 0.92f, 0.92f, 0.92f, 0.49255905f, 0.22369388f, -0.84103900f },
  { 0.90f, 0.90f, 0.90f, -0.42400947f, -0.25435105f, -0.86920738f },
  { 0.88f, 0.88f, 0.88f, 0.45922387f, -0.87484610f, 0.15413558f },
  { 0.87f, 0.87f, 0.87f, -0.44939077f, -0.05239382f, -0.89179754f },
  { 0.87f, 0.87f, 0.87f, 0.02091211f, 0.99145067f, 0.12879559f },
  { 0.86f, 0.86f, 0.86f, 0.34393576f, 0.89501035f, 0.28401536f },
  { 0.84f, 0.84f, 0.84f, -0.91413081f, -0.35632819f, -0.19337822f },
  { 0.82f, 0.82f, 0.82f, -0.34477949f, -0.82641578f, -0.44515619f },
  { 0.81f, 0.81f, 0.81f, -0.39155090f, 0.79122889f, 0.46972838f },
  { 0.81f, 0.81f, 0.81f, 0.83737743f, -0.23360345f, -0.49419478f },
  { 0.79f, 0.79f, 0.79f, 0.45576218f, -0.53630841f, 0.71039015f },
  { 0.79f, 0.79f, 0.79f, -0.49362421f, -0.10432278f, -0.86339557f },
  { 0.77f, 0.77f, 0.77f, -0.86449331f, 0.45788679f, 0.20734258f },
  { 0.75f, 0.75f, 0.75f, -0.22139786f, 0.84642529f, -0.48430070f },
  { 0.73f, 0.73f, 0.73f, -0.09167211f, -0.79229814f, -0.60320795f },
  { 0.73f, 0.73f, 0.73f, 0.15061858f, 0.98238552f, 0.11060166f },
  { 0.73f, 0.73f, 0.73f, 0.12870057f, 0.86849886f, 0.47869188f },
  { 0.72f, 0.72f, 0.72f, -0.53791767f, -0.07360590f, -0.83977783f },
  { 0.72f, 0.72f, 0.72f, -0.25883171f, 0.23061061f, -0.93798983f },
  { 0.72f, 0.72f, 0.72f, 0.10363775f, 0.99439460f, -0.02094242f },
  { 0.71f, 0.71f, 0.71f, 0.60287035f, -0.31976473f, -0.73095679f },
  { 0.70f, 0.70f, 0.70f, -0.45912650f, 0.11504068f, 0.88089073f },
  { 0.70f, 0.70f, 0.70f, -0.54442662f, -0.13078900f, 0.82854927f },
  { 0.70f, 0.70f, 0.70f, 0.40505084f, 0.50168633f, 0.76435900f },
  { 0.69f, 0.69f, 0.69f, 0.08686216f, -0.82069385f, -0.56472701f },
  { 0.69f, 0.69f, 0.69f, -0.36286592f, 0.57215303f, -0.73550612f },
  { 0.69f, 0.69f, 0.69f, -0.26337534f, 0.85611504f, -0.44463518f },
  { 0.69f, 0.69f, 0.69f, -0.07226745f, -0.72777444f, -0.68199837f },
  { 0.69f, 0.69f, 0.69f, -0.58142883f, -0.29476303f, 0.75832397f },
  { 0.69f, 0.69f, 0.69f, -0.29565978f, 0.41252938f, -0.86162919f },
  { 0.68f, 0.68f, 0.68f, 0.00144110f, 0.70772213f, 0.70648944f },
  { 0.68f, 0.68f, 0.68f, -0.85243040f, 0.39757028f, 0.33955887f },
  { 0.68f, 0.68f, 0.68f, 0.32557604f, -0.44146630f, -0.83612663f },
  { 0.68f, 0.68f, 0.68f, -0.15716268f, 0.94635260f, 0.28234145f },
  { 0.68f, 0.68f, 0.68f, -0.10958351f, -0.34063178f, -0.93378878f },
  { 0.68f, 0.68f, 0.68f, -0.38031429f, 0.43474835f, -0.81630564f },
  { 0.67f, 0.67f, 0.67f, -0.09407244f, 0.94666320f, -0.30818695f },
  { 0.67f, 0.67f, 0.67f, -0.77790534f, 0.61004680f, -0.15068571f },
  { 0.67f, 0.67f, 0.67f, -0.34062544f, 0.77780986f, 0.52819133f },
  { 0.67f, 0.67f, 0.67f, 0.81966311f, 0.57047671f, -0.05204546f },
  { 0.67f, 0.67f, 0.67f, -0.45141363f, -0.77794683f, 0.43706343f },
  { 0.67f, 0.67f, 0.67f, 0.77976841f, 0.48331988f, 0.39794865f },
  { 0.66f, 0.66f, 0.66f, 0.93399954f, 0.17978951f, -0.30874035f },
  { 0.66f, 0.66f, 0.66f, 0.01034223f, 0.00802346f, 0.99991435f },
  { 0.66f, 0.66f, 0.66f, 0.08377425f, 0.99590361f, -0.03402735f },
  { 0.66f, 0.66f, 0.66f, 0.77559036f, 0.24355093f, 0.58235943f },
  { 0.66f, 0.66f, 0.66f, 0.05259512f, 0.98439729f, -0.16791590f },
  { 0.66f, 0.66f, 0.66f, 0.21409547f, -0.87054640f, -0.44307119f },
  { 0.66f, 0.66f, 0.66f, 0.87332904f, 0.03195992f, 0.48608121f },
  { 0.66f, 0.66f, 0.66f, -0.10654096f, -0.97021127f, 0.21757543f },
  { 0.66f, 0.66f, 0.66f, -0.68517464f, -0.42269087f, -0.59318477f },
  { 0.66f, 0.66f, 0.66f, -0.20078707f, -0.18514585f, 0.96198004f },
  { 0.65f, 0.65f, 0.65f, -0.44938698f, -0.05242649f, -0.89179754f },
  { 0.65f, 0.65f, 0.65f, 0.63384563f, 0.38041386f, 0.67344266f },
  { 0.65f, 0.65f, 0.65f, 0.64494330f, -0.22627737f, -0.72996348f },
  { 0.64f, 0.64f, 0.64f, -0.96675450f, 0.04615476f, 0.25150633f },
  { 0.64f, 0.64f, 0.64f, 0.51465893f, 0.55278999f, 0.65540016f },
  { 0.64f, 0.64f, 0.64f, -0.64575577f, -0.11827565f, -0.75432777f },
  { 0.63f, 0.63f, 0.63f, 0.12179551f, 0.99254143f, -0.00523596f },
  { 0.63f, 0.63f, 0.63f, 0.44379860f, -0.62094009f, 0.64612401f },
  { 0.63f, 0.63f, 0.63f, 0.54285669f, 0.09698264f, 0.83420676f },
  { 0.63f, 0.63f, 0.63f, -0.00923830f, -0.62267375f, 0.78242707f },
  { 0.63f, 0.63f, 0.63f, -0.52917659f, -0.71961868f, 0.44957885f },
  { 0.63f, 0.63f, 0.63f, -0.53109062f, 0.49525002f, -0.68751013f },
  { 0.62f, 0.62f, 0.62f, -0.38729382f, 0.33341944f, -0.85955513f },
  { 0.62f, 0.62f, 0.62f, -0.39334756f, 0.65734452f, -0.64278764f },
  { 0.62f, 0.62f, 0.62f, -0.53642911f, -0.20573670f, 0.81848413f },
  { 0.62f, 0.62f, 0.62f, 0.51238209f, 0.02050475f, 0.85851276f },
  { 0.62f, 0.62f, 0.62f, -0.53964424f, -0.25130597f, -0.80351067f },
  { 0.62f, 0.62f, 0.62f, -0.46038586f, -0.80009717f, -0.38456386f },
  { 0.62f, 0.62f, 0.62f, -0.24783947f, -0.78804439f, -0.56352603f },
  { 0.62f, 0.62f, 0.62f, -0.51482832f, -0.43944606f, -0.73609710f },
  { 0.61f, 0.61f, 0.61f, -0.57718515f, -0.46531403f, -0.67107385f },
  { 0.60f, 0.60f, 0.60f, -0.53590840f, 0.13896967f, 0.83276027f },
  { 0.60f, 0.60f, 0.60f, 0.73477256f, 0.08463706f, -0.67301250f },
  { 0.60f, 0.60f, 0.60f, -0.66943967f, -0.58699673f, 0.45528594f },
  { 0.60f, 0.60f, 0.60f, 0.81717455f, -0.55024052f, 0.17164253f },
  { 0.60f, 0.60f, 0.60f, -0.05931236f, -0.77451193f, -0.62977242f },
  { 0.59f, 0.59f, 0.59f, -0.20677552f, -0.94014233f, -0.27088046f },
  { 0.59f, 0.59f, 0.59f, -0.59179771f, 0.01597055f, 0.80592829f },
  { 0.59f, 0.59f, 0.59f, 0.35089511f, -0.29815134f, 0.88768148f },
  { 0.59f, 0.59f, 0.59f, -0.31281772f, 0.81403309f, -0.48938245f },
  { 0.59f, 0.59f, 0.59f, 0.55013937f, -0.62063617f, 0.55871052f },
  { 0.58f, 0.58f, 0.58f, -0.44258744f, 0.36446258f, -0.81931883f },
  { 0.58f, 0.58f, 0.58f, 0.93712717f, -0.23032479f, 0.26218918f },
  { 0.58f, 0.58f, 0.58f, 0.69823539f, 0.71228886f, 0.07149744f },
  { 0.58f, 0.58f, 0.58f, -0.59380007f, -0.32756990f, -0.73491460f },
  { 0.57f, 0.57f, 0.57f, 0.85585463f, -0.21424818f, 0.47075528f },
  { 0.57f, 0.57f, 0.57f, -0.91784972f, 0.18632171f, 0.35047984f },
  { 0.57f, 0.57f, 0.57f, 0.47423589f, 0.11977998f, 0.87221158f },
  { 0.57f, 0.57f, 0.57f, -0.34768119f, -0.91950518f, -0.18337947f },
  { 0.57f, 0.57f, 0.57f, 0.11298976f, 0.94522208f, -0.30624920f },
  { 0.57f, 0.57f, 0.57f, -0.63273555f, -0.02306323f, -0.77402443f },
  { 0.57f, 0.57f, 0.57f, -0.95127195f, -0.06575470f, -0.30126059f },
  { 0.57f, 0.57f, 0.57f, 0.23399331f, -0.83502078f, -0.49798331f },
  { 0.56f, 0.56f, 0.56f, -0.64402556f, -0.74742848f, -0.16303897f },
  { 0.56f, 0.56f, 0.56f, -0.45099273f, -0.82574946f, -0.33873791f },
  { 0.56f, 0.56f, 0.56f, 0.07344438f, 0.82496053f, -0.56039810f },
  { 0.56f, 0.56f, 0.56f, 0.82027239f, 0.44838849f, 0.35510695f },
  { 0.56f, 0.56f, 0.56f, -0.55467093f, -0.82453036f, 0.11175800f },
  { 0.56f, 0.56f, 0.56f, 0.00098470f, 0.79652929f, 0.60459912f },
  { 0.56f, 0.56f, 0.56f, -0.51931322f, -0.51270866f, -0.68369848f },
  { 0.56f, 0.56f, 0.56f, -0.90744573f, -0.13717063f, -0.39714789f },
  { 0.55f, 0.55f, 0.55f, -0.61741239f, 0.20428927f, -0.75964981f },
  { 0.55f, 0.55f, 0.55f, 0.46206892f, 0.18158694f, 0.86805445f },
  { 0.55f, 0.55f, 0.55f, -0.83253479f, -0.45524886f, 0.31564903f },
  { 0.55f, 0.55f, 0.55f, 0.07938872f, -0.86398113f, -0.49722639f },
  { 0.55f, 0.55f, 0.55f, 0.22726129f, 0.80580777f, 0.54683280f },
  { 0.55f, 0.55f, 0.55f, -0.35151672f, -0.05753685f, -0.93441182f },
  { 0.55f, 0.55f, 0.55f, -0.10119151f, -0.78901100f, -0.60598838f },
  { 0.55f, 0.55f, 0.55f, -0.26344872f, 0.75281799f, -0.60320795f },
  { 0.55f, 0.55f, 0.55f, 0.43951896f, -0.87913579f, 0.18423727f },
  { 0.55f, 0.55f, 0.55f, -0.19410953f, -0.43589216f, 0.87881714f },
  { 0.55f, 0.55f, 0.55f, -0.44397655f, -0.89373249f, -0.06424203f },
  { 0.54f, 0.54f, 0.54f, -0.70603108f, -0.65198237f, -0.27647609f },
  { 0.54f, 0.54f, 0.54f, -0.75255895f, -0.27608043f, -0.59785837f },
  { 0.54f, 0.54f, 0.54f, 0.10641714f, 0.98896384f, -0.10308187f },
  { 0.54f, 0.54f, 0.54f, -0.40814400f, 0.14263014f, -0.90170676f },
  { 0.54f, 0.54f, 0.54f, -0.44473919f, -0.60738111f, -0.65825164f },
  { 0.54f, 0.54f, 0.54f, 0.22470157f, 0.97039086f, -0.08860456f },
  { 0.54f, 0.54f, 0.54f, -0.07184886f, -0.99423265f, 0.07961901f },
  { 0.54f, 0.54f, 0.54f, -0.35528347f, -0.86002839f, 0.36623058f },
  { 0.54f, 0.54f, 0.54f, -0.07864413f, -0.60621691f, 0.79140139f },
  { 0.53f, 0.53f, 0.53f, 0.21930589f, 0.02474445f, -0.97534233f },
  { 0.53f, 0.53f, 0.53f, -0.48145169f, 0.77386057f, -0.41151434f },
  { 0.53f, 0.53f, 0.53f, -0.28682154f, -0.80198014f, 0.52398592f },
  { 0.53f, 0.53f, 0.53f, -0.51764023f, -0.03426811f, -0.85491186f },
  { 0.53f, 0.53f, 0.53f, 0.80342811f, -0.52658290f, -0.27787358f },
  { 0.53f, 0.53f, 0.53f, -0.31620046f, -0.82247835f, -0.47280711f },
  { 0.53f, 0.53f, 0.53f, 0.96348435f, 0.05569476f, 0.26190847f },
  { 0.53f, 0.53f, 0.53f, -0.94582248f, -0.26311484f, 0.19023786f },
  { 0.53f, 0.53f, 0.53f, 0.10992763f, -0.89637017f, -0.42946059f },
  { 0.53f, 0.53f, 0.53f, 0.12913544f, 0.92617595f, -0.35429105f },
  { 0.53f, 0.53f, 0.53f, -0.23176520f, -0.38250154f, -0.89441460f },
  { 0.53f, 0.53f, 0.53f, -0.08535831f, -0.55945241f, -0.82445556f },
  { 0.53f, 0.53f, 0.53f, 0.44324550f, 0.72425640f, 0.52819133f },
  { 0.53f, 0.53f, 0.53f, 0.44834271f, -0.21264909f, -0.86819881f },
  { 0.52f, 0.52f, 0.52f, 0.41362095f, 0.23584551f, -0.87937170f },
  { 0.52f, 0.52f, 0.52f, 0.31192467f, -0.63275260f, 0.70875043f },
  { 0.52f, 0.52f, 0.52f, 0.49882481f, 0.76434493f, 0.40859598f },
  { 0.52f, 0.52f, 0.52f, -0.37689880f, -0.81943631f, -0.43182340f },
  { 0.52f, 0.52f, 0.52f, -0.76126260f, -0.18992166f, 0.62000728f },
  { 0.52f, 0.52f, 0.52f, 0.79371262f, -0.60046172f, -0.09729340f },
  { 0.52f, 0.52f, 0.52f, 0.38922885f, 0.65979165f, 0.64278764f },
  { 0.52f, 0.52f, 0.52f, 0.27979490f, -0.89055741f, -0.35863951f },
  { 0.52f, 0.52f, 0.52f, -0.09241690f, 0.91913372f, 0.38295215f },
  { 0.52f, 0.52f, 0.52f, -0.23498127f, -0.27736655f, -0.93158555f },
  { 0.52f, 0.52f, 0.52f, -0.36729315f, 0.91887981f, 0.14406836f },
  { 0.52f, 0.52f, 0.52f, -0.45284608f, -0.77534127f, -0.44020039f },
  { 0.51f, 0.51f, 0.51f, -0.13715194f, 0.61950535f, -0.77291816f },
  { 0.51f, 0.51f, 0.51f, 0.87835246f, -0.47798163f, -0.00552685f },
  { 0.51f, 0.51f, 0.51f, -0.07894433f, -0.63949174f, -0.76473403f },
  { 0.51f, 0.51f, 0.51f, 0.41170254f, 0.42931941f, 0.80385685f },
  { 0.51f, 0.51f, 0.51f, -0.95061666f, -0.12451831f, -0.28429425f },
  { 0.51f, 0.51f, 0.51f, 0.81581420f, -0.28489506f, 0.50327134f },
  { 0.51f, 0.51f, 0.51f, 0.49335873f, 0.83783549f, -0.23372820f },
  { 0.50f, 0.50f, 0.50f, -0.76283687f, 0.50562882f, 0.40301293f },
  { 0.50f, 0.50f, 0.50f, 0.18071502f, 0.69835424f, 0.69256294f },
  { 0.50f, 0.50f, 0.50f, 0.09080715f, 0.92820776f, 0.36081082f },
  { 0.50f, 0.50f, 0.50f, -0.86528879f, -0.31031537f, -0.39367452f },
  { 0.50f, 0.50f, 0.50f, 0.27336812f, -0.93157470f, 0.23966326f },
  { 0.50f, 0.50f, 0.50f, -0.17248498f, 0.88873875f, 0.42472619f },
  { 0.50f, 0.50f, 0.50f, 0.02181781f, -0.86194307f, -0.50653547f },
  { 0.50f, 0.50f, 0.50f, -0.92219621f, -0.04080162f, -0.38456386f },
  { 0.50f, 0.50f, 0.50f, 0.69190150f, 0.43881172f, 0.57333815f },
  { 0.50f, 0.50f, 0.50f, 0.67753023f, -0.41546214f, -0.60691357f },
  { 0.50f, 0.50f, 0.50f, -0.69631726f, 0.15543282f, 0.70070177f },
  { 0.50f, 0.50f, 0.50f, -0.07655677f, 0.86205029f, -0.50100732f },
  { 0.50f, 0.50f, 0.50f, 0.37837088f, 0.55527788f, 0.74060917f },
  { 0.50f, 0.50f, 0.50f, -0.04141130f, -0.76361197f, -0.64434606f },
  { 0.50f, 0.50f, 0.50f, -0.24842143f, 0.88034546f, -0.40407753f },
  { 0.50f, 0.50f, 0.50f, -0.68194920f, 0.30964497f, 0.66262007f },
  { 0.49f, 0.49f, 0.49f, -0.61809450f, -0.48327029f, 0.62000728f },
  { 0.49f, 0.49f, 0.49f, -0.19964445f, -0.23947904f, 0.95015359f },
  { 0.49f, 0.49f, 0.49f, -0.36514327f, -0.07475945f, -0.92794472f },
  { 0.49f, 0.49f, 0.49f, 0.55809754f, -0.78956836f, -0.25516450f },
  { 0.49f, 0.49f, 0.49f, 0.11829176f, -0.36111328f, 0.92498881f },
  { 0.49f, 0.49f, 0.49f, 0.34054390f, -0.81492847f, 0.46895781f },
  { 0.48f, 0.48f, 0.48f, 0.43050832f, -0.52426434f, -0.73471731f },
  { 0.48f, 0.48f, 0.48f, 0.06157411f, -0.79870981f, -0.59855765f },
  { 0.48f, 0.48f, 0.48f, -0.68904048f, 0.71727097f, 0.10366054f },
  { 0.48f, 0.48f, 0.48f, 0.03197347f, 0.81077379f, -0.58448571f },
  { 0.48f, 0.48f, 0.48f, -0.91544139f, 0.29032198f, -0.27871174f },
  { 0.48f, 0.48f, 0.48f, -0.43345651f, 0.32896170f, -0.83898729f },
  { 0.48f, 0.48f, 0.48f, -0.14801188f, -0.53949875f, -0.82887489f },
  { 0.48f, 0.48f, 0.48f, -0.52659851f, -0.52270663f, -0.67042661f },
  { 0.48f, 0.48f, 0.48f, -0.45120066f, 0.04785443f, -0.89113855f },
  { 0.48f, 0.48f, 0.48f, -0.17692153f, -0.89012104f, 0.41998014f },
  { 0.48f, 0.48f, 0.48f, -0.23067285f, -0.75293034f, -0.61634892f },
  { 0.48f, 0.48f, 0.48f, -0.63449681f, 0.52747136f, 0.56496704f },
  { 0.47f, 0.47f, 0.47f, -0.47122213f, 0.47445437f, 0.74353397f },
  { 0.47f, 0.47f, 0.47f, -0.35264847f, 0.23098658f, -0.90679890f },
  { 0.47f, 0.47f, 0.47f, -0.15604344f, -0.78537965f, 0.59902358f },
  { 0.47f, 0.47f, 0.47f, 0.17390932f, 0.73164678f, 0.65912712f },
  { 0.47f, 0.47f, 0.47f, -0.11961512f, 0.71928978f, -0.68433505f },
  { 0.47f, 0.47f, 0.47f, 0.26297542f, -0.20426042f, 0.94293243f },
  { 0.47f, 0.47f, 0.47f, -0.32116202f, -0.27551231f, -0.90606177f },
  { 0.47f, 0.47f, 0.47f, -0.49654025f, 0.37129071f, 0.78459609f },
  { 0.47f, 0.47f, 0.47f, -0.09111156f, -0.40102944f, 0.91152292f },
  { 0.47f, 0.47f, 0.47f, 0.17636789f, -0.87337679f, -0.45399049f },
  { 0.47f, 0.47f, 0.47f, 0.29911578f, 0.94651359f, 0.12100314f },
  { 0.47f, 0.47f, 0.47f, 0.64441282f, -0.57549721f, 0.50352269f },
  { 0.47f, 0.47f, 0.47f, 0.21797466f, 0.89871138f, -0.38053244f },
  { 0.47f, 0.47f, 0.47f, -0.05988827f, 0.92193645f, 0.38268343f },
  { 0.47f, 0.47f, 0.47f, -0.03533501f, -0.79732746f, -0.60251170f },
  { 0.47f, 0.47f, 0.47f, -0.26476842f, -0.95042938f, 0.16303897f },
  { 0.47f, 0.47f, 0.47f, -0.48421040f, -0.58409816f, -0.65143657f },
  { 0.46f, 0.46f, 0.46f, 0.21357986f, -0.01931197f, 0.97673470f },
  { 0.46f, 0.46f, 0.46f, -0.42782041f, -0.90014207f, -0.08193851f },
  { 0.46f, 0.46f, 0.46f, -0.27625582f, 0.67330223f, -0.68581831f },
  { 0.46f, 0.46f, 0.46f, 0.54202003f, -0.84024471f, -0.01425304f },
  { 0.46f, 0.46f, 0.46f, 0.21405357f, -0.81399381f, 0.53999555f },
  { 0.46f, 0.46f, 0.46f, 0.84575307f, 0.14659369f, 0.51304197f },
  { 0.46f, 0.46f, 0.46f, 0.09268724f, -0.99442381f, -0.05030242f },
  { 0.46f, 0.46f, 0.46f, 0.20995440f, 0.53300029f, -0.81965226f },
  { 0.46f, 0.46f, 0.46f, -0.09822376f, 0.46013153f, -0.88240075f },
  { 0.46f, 0.46f, 0.46f, -0.76105374f, -0.46812725f, -0.44905910f },
  { 0.46f, 0.46f, 0.46f, 0.92328894f, -0.27064458f, -0.27256012f },
  { 0.46f, 0.46f, 0.46f, 0.14859778f, 0.22715329f, -0.96245521f },
  { 0.45f, 0.45f, 0.45f, -0.14958394f, -0.89387834f, -0.42261827f },
  { 0.45f, 0.45f, 0.45f, 0.65730232f, 0.19496182f, -0.72797221f },
  { 0.45f, 0.45f, 0.45f, -0.32280341f, -0.40196517f, 0.85686755f },
  { 0.45f, 0.45f, 0.45f, -0.54291767f, -0.03661582f, 0.83898729f },
  { 0.45f, 0.45f, 0.45f, -0.43981728f, 0.17766052f, -0.88033944f },
  { 0.45f, 0.45f, 0.45f, -0.30542991f, 0.15269795f, -0.93989146f },
  { 0.45f, 0.45f, 0.45f, 0.25499839f, -0.84816056f, -0.46432686f },
  { 0.45f, 0.45f, 0.45f, -0.08326543f, -0.54733652f, -0.83276027f },
  { 0.45f, 0.45f, 0.45f, 0.19582930f, 0.94011426f, -0.27899113f },
  { 0.45f, 0.45f, 0.45f, -0.62792563f, -0.65061462f, -0.42709485f },
  { 0.45f, 0.45f, 0.45f, -0.15101212f, -0.71274817f, -0.68497109f },
  { 0.44f, 0.44f, 0.44f, 0.20550896f, 0.41407076f, -0.88674206f },
  { 0.44f, 0.44f, 0.44f, -0.94479841f, 0.19114837f, 0.26611695f },
  { 0.44f, 0.44f, 0.44f, -0.00422795f, -0.98544830f, -0.16992286f },
  { 0.44f, 0.44f, 0.44f, 0.15421760f, 0.98716140f, -0.04158502f },
  { 0.44f, 0.44f, 0.44f, -0.41642070f, 0.80608112f, -0.42050806f },
  { 0.44f, 0.44f, 0.44f, -0.29821402f, 0.38770521f, 0.87221158f },
  { 0.44f, 0.44f, 0.44f, -0.19141783f, 0.95578170f, 0.22325011f },
  { 0.44f, 0.44f, 0.44f, 0.46831468f, -0.24158593f, 0.84989268f },
  { 0.44f, 0.44f, 0.44f, -0.45052665f, -0.54978323f, -0.70339471f },
  { 0.44f, 0.44f, 0.44f, -0.91578776f, -0.40152600f, -0.01047178f },
  { 0.44f, 0.44f, 0.44f, 0.36386406f, -0.92986393f, 0.05436927f },
  { 0.44f, 0.44f, 0.44f, -0.66100901f, 0.74200892f, 0.11175800f },
  { 0.44f, 0.44f, 0.44f, 0.38923788f, 0.21221939f, 0.89635754f },
  { 0.44f, 0.44f, 0.44f, -0.96902460f, -0.23980923f, 0.05901600f },
  { 0.44f, 0.44f, 0.44f, -0.43215251f, 0.20825201f, -0.87742537f },
  { 0.43f, 0.43f, 0.43f, 0.92505246f, -0.33007839f, 0.18795274f },
  { 0.43f, 0.43f, 0.43f, -0.39145190f, -0.67892623f, -0.62114781f },
  { 0.43f, 0.43f, 0.43f, -0.41047239f, -0.45701069f, -0.78908408f },
  { 0.43f, 0.43f, 0.43f, 0.05306834f, 0.45894864f, -0.88687646f },
  { 0.43f, 0.43f, 0.43f, -0.38399485f, 0.25597170f, -0.88714510f },
  { 0.43f, 0.43f, 0.43f, 0.67417717f, 0.27358362f, -0.68603003f },
  { 0.43f, 0.43f, 0.43f, -0.05223639f, -0.88358033f, 0.46535707f },
  { 0.43f, 0.43f, 0.43f, 0.37326720f, 0.88652366f, 0.27339965f },
  { 0.43f, 0.43f, 0.43f, -0.66319853f, -0.34340242f, -0.66501313f },
  { 0.43f, 0.43f, 0.43f, 0.48571777f, 0.84696811f, 0.21615562f },
  { 0.43f, 0.43f, 0.43f, 0.76590681f, 0.41189557f, 0.49368894f },
  { 0.43f, 0.43f, 0.43f, 0.54339010f, 0.53515434f, -0.64678979f },
  { 0.43f, 0.43f, 0.43f, 0.31167477f, -0.35451192f, 0.88157821f },
  { 0.43f, 0.43f, 0.43f, 0.26600966f, -0.30345955f, -0.91495967f },
  { 0.43f, 0.43f, 0.43f, 0.18100056f, -0.81531858f, 0.54999495f },
  { 0.43f, 0.43f, 0.43f, 0.28402635f, -0.95503008f, -0.08512709f },
  { 0.43f, 0.43f, 0.43f, -0.82599336f, 0.39974546f, 0.39741483f },
  { 0.43f, 0.43f, 0.43f, 0.52045333f, 0.11323932f, 0.84634811f },
  { 0.43f, 0.43f, 0.43f, -0.38155916f, 0.34668523f, -0.85686755f },
  { 0.43f, 0.43f, 0.43f, 0.94048357f, 0.29022917f, -0.17679845f },
  { 0.42f, 0.42f, 0.42f, -0.65976042f, 0.31787598f, 0.68093395f },
  { 0.42f, 0.42f, 0.42f, 0.75550532f, 0.65271008f, 0.05640237f },
  { 0.42f, 0.42f, 0.42f, -0.82416737f, 0.15095326f, 0.54585826f },
  { 0.42f, 0.42f, 0.42f, -0.23505063f, 0.85165226f, -0.46844387f },
  { 0.42f, 0.42f, 0.42f, -0.29367635f, 0.52556175f, -0.79846042f },
  { 0.42f, 0.42f, 0.42f, -0.65471596f, -0.33943284f, -0.67537570f },
  { 0.42f, 0.42f, 0.42f, -0.80977929f, 0.56097937f, 0.17192909f },
  { 0.42f, 0.42f, 0.42f, -0.54960877f, -0.62947369f, 0.54926592f },
  { 0.42f, 0.42f, 0.42f, 0.59488690f, -0.19171357f, -0.78061223f },
  { 0.42f, 0.42f, 0.42f, 0.46696907f, -0.81896025f, 0.33353263f },
  { 0.42f, 0.42f, 0.42f, -0.53401983f, -0.54318553f, 0.64789838f },
  { 0.42f, 0.42f, 0.42f, 0.86717367f, -0.27334976f, 0.41628081f },
  { 0.42f, 0.42f, 0.42f, -0.19042586f, -0.94975334f, 0.24840812f },
  { 0.42f, 0.42f, 0.42f, 0.08164751f, -0.69026470f, -0.71893555f },
  { 0.42f, 0.42f, 0.42f, 0.47126061f, -0.88182133f, 0.01745241f },
  { 0.42f, 0.42f, 0.42f, 0.86414129f, 0.42178112f, -0.27451870f },
  { 0.42f, 0.42f, 0.42f, 0.23254204f, -0.90350789f, -0.35999683f },
  { 0.42f, 0.42f, 0.42f, 0.38470000f, -0.12188006f, 0.91495967f },
  { 0.42f, 0.42f, 0.42f, 0.53821993f, 0.56313294f, 0.62705708f },
  { 0.41f, 0.41f, 0.41f, 0.88222283f, -0.45827830f, 0.10799935f },
  { 0.41f, 0.41f, 0.41f, -0.55384320f, 0.81712651f, 0.15988119f },
  { 0.41f, 0.41f, 0.41f, -0.31759623f, 0.87120867f, 0.37433687f },
  { 0.41f, 0.41f, 0.41f, -0.84410006f, 0.45197052f, 0.28847480f },
  { 0.41f, 0.41f, 0.41f, -0.25677931f, -0.73427385f, 0.62841570f },
  { 0.41f, 0.41f, 0.41f, 0.55381840f, 0.81517404f, -0.16963618f },
  { 0.41f, 0.41f, 0.41f, 0.36669397f, 0.87038088f, 0.32859191f },
  { 0.41f, 0.41f, 0.41f, -0.49786922f, 0.29659346f, -0.81495923f },
  { 0.41f, 0.41f, 0.41f, -0.09407933f, -0.95949417f, -0.26555610f },
  { 0.41f, 0.41f, 0.41f, -0.84354466f, 0.09984009f, -0.52769727f },
  { 0.41f, 0.41f, 0.41f, 0.05501411f, 0.96510792f, -0.25600818f },
  { 0.41f, 0.41f, 0.41f, 0.98458898f, 0.08368783f, -0.15356073f },
  { 0.41f, 0.41f, 0.41f, 0.51513886f, 0.34910315f, -0.78278923f },
  { 0.41f, 0.41f, 0.41f, 0.35807529f, 0.74987817f, -0.55629563f },
  { 0.41f, 0.41f, 0.41f, -0.51334995f, -0.62172925f, -0.59154421f },
  { 0.41f, 0.41f, 0.41f, -0.53775209f, -0.84099400f, -0.05959675f },
  { 0.41f, 0.41f, 0.41f, -0.56938469f, -0.39647010f, -0.72014755f },
  { 0.41f, 0.41f, 0.41f, 0.02724542f, -0.29556635f, 0.95493364f },
  { 0.41f, 0.41f, 0.41f, -0.09863980f, 0.31889999f, -0.94264150f },
  { 0.41f, 0.41f, 0.41f, -0.40082386f, 0.81773162f, 0.41310441f },
  { 0.41f, 0.41f, 0.41f, 0.55308396f, -0.80436671f, -0.21700758f },
  { 0.41f, 0.41f, 0.41f, -0.22924352f, -0.75374138f, -0.61589068f },
  { 0.41f, 0.41f, 0.41f, -0.95178890f, 0.17068234f, -0.25488323f },
  { 0.41f, 0.41f, 0.41f, -0.48841876f, 0.47324172f, 0.73313665f },
  { 0.41f, 0.41f, 0.41f, -0.32039970f, 0.90352762f, 0.28457311f },
  { 0.41f, 0.41f, 0.41f, -0.68017197f, -0.53061265f, 0.50578284f },
  { 0.41f, 0.41f, 0.41f, 0.21505727f, -0.34178066f, -0.91484225f },
  { 0.41f, 0.41f, 0.41f, 0.60158956f, 0.27410734f, 0.75030339f },
  { 0.40f, 0.40f, 0.40f, 0.18263824f, 0.97591895f, -0.11927044f },
  { 0.40f, 0.40f, 0.40f, -0.60498190f, 0.46129021f, -0.64900553f },
  { 0.40f, 0.40f, 0.40f, 0.61407155f, -0.74791521f, 0.25206935f },
  { 0.40f, 0.40f, 0.40f, -0.51517010f, -0.71572614f, -0.47152501f },
  { 0.40f, 0.40f, 0.40f, -0.06157968f, -0.48574832f, -0.87192690f },
  { 0.40f, 0.40f, 0.40f, 0.61877620f, 0.76971698f, 0.15700907f },
  { 0.40f, 0.40f, 0.40f, -0.34942970f, 0.70649987f, -0.61543220f },
  { 0.40f, 0.40f, 0.40f, -0.49179676f, -0.04598343f, -0.86949492f },
  { 0.40f, 0.40f, 0.40f, -0.18932696f, 0.80746388f, 0.55871052f },
  { 0.40f, 0.40f, 0.40f, 0.06259237f, 0.92209119f, -0.38187706f },
  { 0.40f, 0.40f, 0.40f, 0.92404860f, 0.35478613f, -0.14234102f },
  { 0.40f, 0.40f, 0.40f, 0.65539765f, 0.60047364f, 0.45813251f },
  { 0.40f, 0.40f, 0.40f, -0.86757469f, 0.44882801f, -0.21416709f },
  { 0.40f, 0.40f, 0.40f, -0.99870187f, 0.04054843f, 0.03082926f },
  { 0.40f, 0.40f, 0.40f, -0.30486819f, -0.08492956f, -0.94860023f },
  { 0.40f, 0.40f, 0.40f, -0.09859344f, 0.31891432f, -0.94264150f },
  { 0.40f, 0.40f, 0.40f, 0.88851464f, 0.37479037f, 0.26471469f },
  { 0.40f, 0.40f, 0.40f, -0.38818985f, 0.46139854f, -0.79775935f },
  { 0.40f, 0.40f, 0.40f, 0.71581513f, -0.18540023f, 0.67322761f },
  { 0.40f, 0.40f, 0.40f, -0.02655304f, -0.42600515f, -0.90433097f },
  { 0.39f, 0.39f, 0.39f, -0.39449832f, 0.02481967f, -0.91856140f },
  { 0.39f, 0.39f, 0.39f, 0.49345389f, 0.76838762f, 0.40753371f },
  { 0.39f, 0.39f, 0.39f, 0.40777862f, 0.87249523f, 0.26920000f },
  { 0.39f, 0.39f, 0.39f, -0.20809567f, -0.70914137f, -0.67365772f },
  { 0.39f, 0.39f, 0.39f, 0.58154839f, 0.09462435f, 0.80798990f },
  { 0.39f, 0.39f, 0.39f, 0.36152646f, -0.37826082f, -0.85218388f },
  { 0.39f, 0.39f, 0.39f, -0.37042993f, -0.22345768f, 0.90158099f },
  { 0.39f, 0.39f, 0.39f, -0.50194037f, -0.70804209f, -0.49672151f },
  { 0.39f, 0.39f, 0.39f, -0.36118892f, 0.27333024f, 0.89153415f },
  { 0.39f, 0.39f, 0.39f, 0.92191952f, -0.38738132f, -0.00029089f },
  { 0.39f, 0.39f, 0.39f, 0.10666521f, 0.97921681f, 0.17250219f },
  { 0.39f, 0.39f, 0.39f, 0.78493893f, -0.54919469f, -0.28680322f },
  { 0.39f, 0.39f, 0.39f, 0.90994221f, -0.20402180f, -0.36108211f },
  { 0.39f, 0.39f, 0.39f, -0.53143394f, -0.80430645f, 0.26583657f },
  { 0.39f, 0.39f, 0.39f, 0.01856304f, -0.64118099f, -0.76716512f },
  { 0.39f, 0.39f, 0.39f, -0.53849858f, -0.68811131f, 0.48633540f },
  { 0.39f, 0.39f, 0.39f, -0.98320395f, -0.18074743f, -0.02530457f },
  { 0.39f, 0.39f, 0.39f, 0.29422757f, 0.95074064f, 0.09758290f },
  { 0.38f, 0.38f, 0.38f, 0.54333377f, 0.30101991f, -0.78369343f },
  { 0.38f, 0.38f, 0.38f, -0.54781771f, 0.63251108f, -0.54756325f },
  { 0.38f, 0.38f, 0.38f, 0.98097295f, -0.18550418f, 0.05727362f },
  { 0.38f, 0.38f, 0.38f, -0.00850191f, -0.87245458f, 0.48862123f },
  { 0.38f, 0.38f, 0.38f, 0.59852648f, 0.71024871f, -0.37055743f },
  { 0.38f, 0.38f, 0.38f, 0.50751889f, 0.75859994f, 0.40859598f },
  { 0.38f, 0.38f, 0.38f, -0.67069167f, 0.04087443f, 0.74060917f },
  { 0.38f, 0.38f, 0.38f, 0.47919452f, -0.87060165f, 0.11146893f },
  { 0.38f, 0.38f, 0.38f, -0.53265584f, -0.84271425f, 0.07816910f },
  { 0.38f, 0.38f, 0.38f, 0.01522965f, 0.96946830f, -0.24474335f },
  { 0.38f, 0.38f, 0.38f, -0.35717896f, 0.67022651f, -0.65055329f },
  { 0.38f, 0.38f, 0.38f, 0.00118872f, 0.58377624f, 0.81191373f },
  { 0.38f, 0.38f, 0.38f, 0.28271061f, 0.95825225f, 0.04274754f },
  { 0.38f, 0.38f, 0.38f, 0.60961920f, 0.77445608f, 0.16906281f },
  { 0.38f, 0.38f, 0.38f, 0.86973280f, 0.45977125f, -0.17937458f },
  { 0.38f, 0.38f, 0.38f, 0.52006382f, -0.49814272f, 0.69382089f },
  { 0.38f, 0.38f, 0.38f, 0.03161889f, -0.98558581f, 0.16619509f },
  { 0.38f, 0.38f, 0.38f, 0.59160018f, -0.51973379f, 0.61634892f },
  { 0.38f, 0.38f, 0.38f, 0.09249251f, 0.99467951f, -0.04536299f },
  { 0.38f, 0.38f, 0.38f, -0.74782109f, -0.66307187f, 0.03315518f },
  { 0.38f, 0.38f, 0.38f, 0.59040618f, 0.79017019f, -0.16447377f },
  { 0.38f, 0.38f, 0.38f, -0.39199558f, -0.85949266f, 0.32804239f },
  { 0.38f, 0.38f, 0.38f, -0.62340361f, 0.23525384f, -0.74566984f },
  { 0.38f, 0.38f, 0.38f, 0.72514129f, -0.57336760f, -0.38133928f },
  { 0.38f, 0.38f, 0.38f, -0.05271227f, -0.99748433f, 0.04739701f },
  { 0.38f, 0.38f, 0.38f, -0.48997906f, 0.47254783f, -0.73254287f },
  { 0.38f, 0.38f, 0.38f, -0.01542062f, -0.54637170f, 0.83740085f },
  { 0.38f, 0.38f, 0.38f, 0.59171128f, -0.24122198f, 0.76921374f },
  { 0.38f, 0.38f, 0.38f, -0.15511373f, -0.49034050f, -0.85761642f },
  { 0.37f, 0.37f, 0.37f, 0.25864050f, -0.89218408f, -0.37028724f },
  { 0.37f, 0.37f, 0.37f, 0.79617882f, -0.42780229f, 0.42788377f },
  { 0.37f, 0.37f, 0.37f, 0.39189056f, 0.86928928f, 0.30126059f },
  { 0.37f, 0.37f, 0.37f, -0.24029249f, 0.32552803f, -0.91448951f },
  { 0.37f, 0.37f, 0.37f, 0.94871718f, -0.28726330f, -0.13196805f },
  { 0.37f, 0.37f, 0.37f, 0.61701846f, -0.73761857f, 0.27423897f },
  { 0.37f, 0.37f, 0.37f, 0.40852863f, 0.61256278f, 0.67666179f },
  { 0.37f, 0.37f, 0.37f, 0.41218764f, 0.38002291f, 0.82806033f },
  { 0.37f, 0.37f, 0.37f, 0.65894055f, -0.73386419f, -0.16504760f },
  { 0.37f, 0.37f, 0.37f, 0.23625541f, -0.57248890f, 0.78513682f },
  { 0.37f, 0.37f, 0.37f, 0.19701055f, -0.56322998f, 0.80247045f },
  { 0.37f, 0.37f, 0.37f, 0.17973220f, -0.12412395f, -0.97585326f }
};
const unsigned int NumStars = countof(stars);

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
