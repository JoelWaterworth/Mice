// Fill out your copyright notice in the Description page of Project Settings.

#include "GridLib.h"


FVector2DInt UGridLib::Add_FVector2DIntFVector2DInt(const FVector2DInt &arg1, const FVector2DInt &arg2)
{
	return FVector2DInt(arg1.x + arg2.x, arg1.y + arg2.y);
}

FVector2DInt UGridLib::Add_FVector2DIntInt(const FVector2DInt & arg1, const int32 & arg2)
{
	return FVector2DInt(arg1.x + arg2, arg1.y + arg2);
}

FVector2DInt UGridLib::Mul_FVector2DIntInt(const FVector2DInt & arg1, const int32 & arg2)
{
	return FVector2DInt(arg1.x * arg2, arg1.y * arg2);
}

bool UGridLib::EqualEqual_FVector2DIntFVector2DInt(const FVector2DInt & arg1, const FVector2DInt & arg2)
{
	return arg1 == arg2;
}
