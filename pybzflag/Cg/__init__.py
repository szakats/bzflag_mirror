""" Cg

This is a python package providing a wrapper to nVidia's Cg runtime
"""
#
# Python Cg Package
# Copyright (C) 2003 Micah Dowty <micahjd@users.sourceforge.net>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

# yay for constants
CG_UNKNOWN_TYPE     = 0
CG_STRUCT           = 1
CG_ARRAY            = 2
CG_TYPE_START_ENUM  = 1024
CG_HALF             = 1025
CG_HALF2            = 1026
CG_HALF3            = 1027
CG_HALF4            = 1028
CG_HALF1x1          = 1029
CG_HALF1x2          = 1030
CG_HALF1x3          = 1031
CG_HALF1x4          = 1032
CG_HALF2x1          = 1033
CG_HALF2x2          = 1034
CG_HALF2x3          = 1035
CG_HALF2x4          = 1036
CG_HALF3x1          = 1037
CG_HALF3x2          = 1038
CG_HALF3x3          = 1039
CG_HALF3x4          = 1040
CG_HALF4x1          = 1041
CG_HALF4x2          = 1042
CG_HALF4x3          = 1043
CG_HALF4x4          = 1044
CG_FLOAT            = 1045
CG_FLOAT2           = 1046
CG_FLOAT3           = 1047
CG_FLOAT4           = 1048
CG_FLOAT1x1         = 1049
CG_FLOAT1x2         = 1050
CG_FLOAT1x3         = 1051
CG_FLOAT1x4         = 1052
CG_FLOAT2x1         = 1053
CG_FLOAT2x2         = 1054
CG_FLOAT2x3         = 1055
CG_FLOAT2x4         = 1056
CG_FLOAT3x1         = 1057
CG_FLOAT3x2         = 1058
CG_FLOAT3x3         = 1059
CG_FLOAT3x4         = 1060
CG_FLOAT4x1         = 1061
CG_FLOAT4x2         = 1062
CG_FLOAT4x3         = 1063
CG_FLOAT4x4         = 1064

CG_UNKNOWN          = 4096
CG_IN               = 4097
CG_OUT              = 4098
CG_INOUT            = 4099
CG_MIXED            = 4100
CG_VARYING          = 4101
CG_UNIFORM          = 4102
CG_CONSTANT         = 4103
CG_PROGRAM_SOURCE   = 4104
CG_PROGRAM_ENTRY    = 4105
CG_COMPILED_PROGRAM = 4106
CG_PROGRAM_PROFILE  = 4107
CG_GLOBAL           = 4108
CG_PROGRAM          = 4109
CG_DEFAULT          = 4110
CG_ERROR            = 4111
CG_SOURCE           = 4112
CG_OBJECT           = 4113
