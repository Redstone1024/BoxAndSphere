// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedMatrix.h"

const FFixedMatrix FFixedMatrix::Identity = FFixedMatrix(FFixedPlane(1, 0, 0, 0), FFixedPlane(0, 1, 0, 0), FFixedPlane(0, 0, 1, 0), FFixedPlane(0, 0, 0, 1));
