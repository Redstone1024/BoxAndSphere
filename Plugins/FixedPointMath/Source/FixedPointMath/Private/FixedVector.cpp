// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedVector.h"

const FFixedVector FFixedVector::ZeroVector = FFixedVector(0);
const FFixedVector FFixedVector::OneVector = FFixedVector(1);
const FFixedVector FFixedVector::UpVector = FFixedVector(0, 0, 1);
const FFixedVector FFixedVector::DownVector = FFixedVector(0, 0, -1);
const FFixedVector FFixedVector::ForwardVector = FFixedVector(1, 0, 0);
const FFixedVector FFixedVector::BackwardVector = FFixedVector(-1, 0, 0);
const FFixedVector FFixedVector::RightVector = FFixedVector(0, 1, 0);
const FFixedVector FFixedVector::LeftVector = FFixedVector(0, -1, 0);
