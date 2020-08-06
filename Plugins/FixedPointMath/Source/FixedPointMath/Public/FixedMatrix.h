// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Fixed.h"
#include "FixedPlane.h"
#include "FixedPointMath.h"
#include "FixedMatrix.generated.h"

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedMatrixData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Matrix", SaveGame)
	FFixedPlane XPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Matrix", SaveGame)
	FFixedPlane YPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Matrix", SaveGame)
	FFixedPlane ZPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Matrix", SaveGame)
	FFixedPlane WPlane;

	FORCEINLINE FFixedPlane& operator [](int32 Index) { check(Index >= 0 && Index < 4); return (&XPlane)[Index]; }
	FORCEINLINE FFixedPlane operator [](int32 Index) const { check(Index >= 0 && Index < 4); return (&XPlane)[Index]; }
};

USTRUCT(BlueprintType)
struct FIXEDPOINTMATH_API FFixedMatrix
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fixed Matrix", SaveGame)
	FFixedMatrixData M;

	static const FFixedMatrix ZeroMatrix;
	static const FFixedMatrix Identity;

	FORCEINLINE FFixedMatrix() { }
	FORCEINLINE FFixedMatrix(const FFixedPlane& InX, const FFixedPlane& InY, const FFixedPlane& InZ, const FFixedPlane& InW) { M.XPlane = InX; M.YPlane = InY; M.ZPlane = InZ; M.WPlane = InW; }
	FORCEINLINE FFixedMatrix(const FFixedVector& InX, const FFixedVector& InY, const FFixedVector& InZ, const FFixedVector& InW);
	
	explicit FORCEINLINE FFixedMatrix(const FMatrix& InMatrix);
	explicit FORCEINLINE operator FMatrix() const;

	inline bool operator ==(const FFixedMatrix& Other) const { return (M.XPlane == Other.M.XPlane) && (M.YPlane == Other.M.YPlane) && (M.ZPlane == Other.M.ZPlane) && (M.WPlane == Other.M.WPlane); }
	inline bool operator !=(const FFixedMatrix& Other) const { return (M.XPlane != Other.M.XPlane) || (M.YPlane != Other.M.YPlane) || (M.ZPlane != Other.M.ZPlane) || (M.WPlane != Other.M.WPlane); }
	friend inline FFixedMatrix operator *(const FFixedMatrix& A, const FFixedMatrix& B);
	friend inline FFixedMatrix operator +(const FFixedMatrix& A, const FFixedMatrix& B) { return FFixedMatrix(A.M.XPlane + B.M.XPlane, A.M.YPlane + B.M.YPlane, A.M.ZPlane + B.M.ZPlane, A.M.WPlane + B.M.WPlane); }
	friend inline FFixedMatrix operator -(const FFixedMatrix& A, const FFixedMatrix& B) { return FFixedMatrix(A.M.XPlane - B.M.XPlane, A.M.YPlane - B.M.YPlane, A.M.ZPlane - B.M.ZPlane, A.M.WPlane - B.M.WPlane); }
	inline FFixedMatrix operator *(FFixed Other) const { return FFixedMatrix(M.XPlane * Other, M.YPlane * Other, M.ZPlane * Other, M.WPlane * Other); }
	inline FFixedMatrix operator /(FFixed Other) const { return FFixedMatrix(M.XPlane / Other, M.YPlane / Other, M.ZPlane / Other, M.WPlane / Other); }
	inline FFixedMatrix& operator *=(const FFixedMatrix& Other) { *this = *this * Other; return *this; }
	inline FFixedMatrix& operator +=(const FFixedMatrix& Other) { M.XPlane += Other.M.XPlane; M.YPlane += Other.M.YPlane; M.ZPlane += Other.M.ZPlane; M.WPlane += Other.M.WPlane; return *this; }
	inline FFixedMatrix& operator -=(const FFixedMatrix& Other) { M.XPlane -= Other.M.XPlane; M.YPlane -= Other.M.YPlane; M.ZPlane -= Other.M.ZPlane; M.WPlane -= Other.M.WPlane; return *this; }
	inline FFixedMatrix& operator *=(FFixed Other) { M.XPlane *= Other; M.YPlane *= Other; M.ZPlane *= Other; M.WPlane *= Other; return *this; }
	inline FFixedMatrix& operator /=(FFixed Other) { M.XPlane /= Other; M.YPlane /= Other; M.ZPlane /= Other; M.WPlane /= Other; return *this; }

	inline void SetIdentity() { M = Identity.M; }
	inline FFixedVector TransformPosition(const FFixedVector &V) const { return V.X * M[0] + V.Y * M[1] + V.Z * M[2] + M[3]; }
	inline FFixedVector InverseTransformPosition(const FFixedVector &V) const { FFixedMatrix InvSelf = Inverse(); return InvSelf.TransformPosition(V); }
	inline FFixedVector TransformVector(const FFixedVector& V) const { return V.X * M[0] + V.Y * M[1] + V.Z * M[2]; }
	inline FFixedVector InverseTransformVector(const FFixedVector &V) const { FFixedMatrix InvSelf = Inverse(); return InvSelf.TransformVector(V); }
	inline FFixedMatrix GetTransposed() const;
	inline FFixed Determinant() const;
	inline FFixed RotDeterminant() const;
	inline FFixedMatrix Inverse() const;
	inline FFixedMatrix TransposeAdjoint() const;
	inline void RemoveScaling();
	inline FFixedMatrix GetMatrixWithoutScale() const { FFixedMatrix Result = *this; Result.RemoveScaling(); return Result; }
	inline FFixedVector ExtractScaling();
	inline FFixedVector GetScaleVector() const;
	inline FFixedMatrix RemoveTranslation() const;
	inline FFixedMatrix ConcatTranslation(const FFixedVector& Translation) const;
	inline void ScaleTranslation(const FFixedVector& Scale3D);
	inline FFixed GetMaximumAxisScale() const;
	inline FFixedMatrix ApplyScale(FFixed Scale) const;
	inline FFixedVector GetOrigin() const { return FFixedVector(M[3][0], M[3][1], M[3][2]); }
	inline FFixedVector GetScaledAxis(EAxis::Type Axis) const;
	inline void GetScaledAxes(FFixedVector &X, FFixedVector &Y, FFixedVector &Z) const;
	inline FFixedVector GetUnitAxis(EAxis::Type Axis) const { return FFixedMath::Normalize(GetScaledAxis(Axis)); }
	inline void GetUnitAxes(FFixedVector &X, FFixedVector &Y, FFixedVector &Z) const { GetScaledAxes(X, Y, Z); X.Normalize(); Y.Normalize(); Z.Normalize(); }
	inline void SetAxis(int32 i, const FFixedVector& Axis) { checkSlow(i >= 0 && i <= 2); M[i][0] = Axis.X; M[i][1] = Axis.Y; M[i][2] = Axis.Z; }
	inline void SetOrigin(const FFixedVector& NewOrigin) { M[3][0] = NewOrigin.X; M[3][1] = NewOrigin.Y; M[3][2] = NewOrigin.Z; }
	inline void SetAxes(FFixedVector* Axis0 = nullptr, FFixedVector* Axis1 = nullptr, FFixedVector* Axis2 = nullptr, FFixedVector* Origin = nullptr);
};

FORCEINLINE FFixedMatrix::FFixedMatrix(const FFixedVector & InX, const FFixedVector & InY, const FFixedVector & InZ, const FFixedVector & InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = 0;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = 0;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = 0;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = 1;
}

FORCEINLINE FFixedMatrix::FFixedMatrix(const FMatrix & InMatrix)
{
	M[0][0] = static_cast<FFixed>(InMatrix.M[0][0]);
	M[0][1] = static_cast<FFixed>(InMatrix.M[1][0]);
	M[0][2] = static_cast<FFixed>(InMatrix.M[2][0]);
	M[0][3] = static_cast<FFixed>(InMatrix.M[3][0]);

	M[1][0] = static_cast<FFixed>(InMatrix.M[0][1]);
	M[1][1] = static_cast<FFixed>(InMatrix.M[1][1]);
	M[1][2] = static_cast<FFixed>(InMatrix.M[2][1]);
	M[1][3] = static_cast<FFixed>(InMatrix.M[3][1]);

	M[2][0] = static_cast<FFixed>(InMatrix.M[0][2]);
	M[2][1] = static_cast<FFixed>(InMatrix.M[1][2]);
	M[2][2] = static_cast<FFixed>(InMatrix.M[2][2]);
	M[2][3] = static_cast<FFixed>(InMatrix.M[3][2]);

	M[3][0] = static_cast<FFixed>(InMatrix.M[0][3]);
	M[3][1] = static_cast<FFixed>(InMatrix.M[1][3]);
	M[3][2] = static_cast<FFixed>(InMatrix.M[2][3]);
	M[3][3] = static_cast<FFixed>(InMatrix.M[3][3]);
}

FORCEINLINE FFixedMatrix::operator FMatrix() const
{
	FMatrix Result;

	Result.M[0][0] = static_cast<float>(M[0][0]);
	Result.M[0][1] = static_cast<float>(M[1][0]);
	Result.M[0][2] = static_cast<float>(M[2][0]);
	Result.M[0][3] = static_cast<float>(M[3][0]);

	Result.M[1][0] = static_cast<float>(M[0][1]);
	Result.M[1][1] = static_cast<float>(M[1][1]);
	Result.M[1][2] = static_cast<float>(M[2][1]);
	Result.M[1][3] = static_cast<float>(M[3][1]);

	Result.M[2][0] = static_cast<float>(M[0][2]);
	Result.M[2][1] = static_cast<float>(M[1][2]);
	Result.M[2][2] = static_cast<float>(M[2][2]);
	Result.M[2][3] = static_cast<float>(M[3][2]);

	Result.M[3][0] = static_cast<float>(M[0][3]);
	Result.M[3][1] = static_cast<float>(M[1][3]);
	Result.M[3][2] = static_cast<float>(M[2][3]);
	Result.M[3][3] = static_cast<float>(M[3][3]);

	return Result;
}

FORCEINLINE FFixedMatrix operator *(const FFixedMatrix & A, const FFixedMatrix & B)
{
	FFixedMatrix Result;

	Result.M[0][0] += A.M[0][0] * B.M[0][0] + A.M[0][1] * B.M[1][0] + A.M[0][2] * B.M[2][0] + A.M[0][3] * B.M[3][0];
	Result.M[0][1] += A.M[0][0] * B.M[0][1] + A.M[0][1] * B.M[1][1] + A.M[0][2] * B.M[2][1] + A.M[0][3] * B.M[3][1];
	Result.M[0][2] += A.M[0][0] * B.M[0][2] + A.M[0][1] * B.M[1][2] + A.M[0][2] * B.M[2][2] + A.M[0][3] * B.M[3][2];
	Result.M[0][3] += A.M[0][0] * B.M[0][3] + A.M[0][1] * B.M[1][3] + A.M[0][2] * B.M[2][3] + A.M[0][3] * B.M[3][3];

	Result.M[1][0] += A.M[1][0] * B.M[0][0] + A.M[1][1] * B.M[1][0] + A.M[1][2] * B.M[2][0] + A.M[1][3] * B.M[3][0];
	Result.M[1][1] += A.M[1][0] * B.M[0][1] + A.M[1][1] * B.M[1][1] + A.M[1][2] * B.M[2][1] + A.M[1][3] * B.M[3][1];
	Result.M[1][2] += A.M[1][0] * B.M[0][2] + A.M[1][1] * B.M[1][2] + A.M[1][2] * B.M[2][2] + A.M[1][3] * B.M[3][2];
	Result.M[1][3] += A.M[1][0] * B.M[0][3] + A.M[1][1] * B.M[1][3] + A.M[1][2] * B.M[2][3] + A.M[1][3] * B.M[3][3];

	Result.M[2][0] += A.M[2][0] * B.M[0][0] + A.M[2][1] * B.M[1][0] + A.M[2][2] * B.M[2][0] + A.M[2][3] * B.M[3][0];
	Result.M[2][1] += A.M[2][0] * B.M[0][1] + A.M[2][1] * B.M[1][1] + A.M[2][2] * B.M[2][1] + A.M[2][3] * B.M[3][1];
	Result.M[2][2] += A.M[2][0] * B.M[0][2] + A.M[2][1] * B.M[1][2] + A.M[2][2] * B.M[2][2] + A.M[2][3] * B.M[3][2];
	Result.M[2][3] += A.M[2][0] * B.M[0][3] + A.M[2][1] * B.M[1][3] + A.M[2][2] * B.M[2][3] + A.M[2][3] * B.M[3][3];

	Result.M[3][0] += A.M[3][0] * B.M[0][0] + A.M[3][1] * B.M[1][0] + A.M[3][2] * B.M[2][0] + A.M[3][3] * B.M[3][0];
	Result.M[3][1] += A.M[3][0] * B.M[0][1] + A.M[3][1] * B.M[1][1] + A.M[3][2] * B.M[2][1] + A.M[3][3] * B.M[3][1];
	Result.M[3][2] += A.M[3][0] * B.M[0][2] + A.M[3][1] * B.M[1][2] + A.M[3][2] * B.M[2][2] + A.M[3][3] * B.M[3][2];
	Result.M[3][3] += A.M[3][0] * B.M[0][3] + A.M[3][1] * B.M[1][3] + A.M[3][2] * B.M[2][3] + A.M[3][3] * B.M[3][3];

	return Result;
}

inline FFixedMatrix FFixedMatrix::GetTransposed() const
{
	FFixedMatrix Result;

	Result.M[0][0] = M[0][0];
	Result.M[0][1] = M[1][0];
	Result.M[0][2] = M[2][0];
	Result.M[0][3] = M[3][0];

	Result.M[1][0] = M[0][1];
	Result.M[1][1] = M[1][1];
	Result.M[1][2] = M[2][1];
	Result.M[1][3] = M[3][1];

	Result.M[2][0] = M[0][2];
	Result.M[2][1] = M[1][2];
	Result.M[2][2] = M[2][2];
	Result.M[2][3] = M[3][2];

	Result.M[3][0] = M[0][3];
	Result.M[3][1] = M[1][3];
	Result.M[3][2] = M[2][3];
	Result.M[3][3] = M[3][3];

	return Result;
}

inline FFixed FFixedMatrix::Determinant() const
{
	return	M[0][0] * (
				M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
				M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
			) -
			M[1][0] * (
				M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			) +
			M[2][0] * (
				M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			) -
			M[3][0] * (
				M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}

inline FFixed FFixedMatrix::RotDeterminant() const
{
	return
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
}

inline FFixedMatrix FFixedMatrix::Inverse() const
{
	if (GetScaledAxis(EAxis::X).IsZero() &&
		GetScaledAxis(EAxis::Y).IsZero() &&
		GetScaledAxis(EAxis::Z).IsZero())
		return FFixedMatrix::Identity;

	if (Determinant() == 0) return FFixedMatrix::Identity;

	FFixedMatrix Return;
	FFixedMatrixData& Result = Return.M;
	FFixed Det[4];
	FFixedMatrixData Tmp;

	Tmp[0][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Tmp[0][1] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Tmp[0][2] = M[1][2] * M[2][3] - M[1][3] * M[2][2];

	Tmp[1][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Tmp[1][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Tmp[1][2] = M[0][2] * M[2][3] - M[0][3] * M[2][2];

	Tmp[2][0] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Tmp[2][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Tmp[2][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Tmp[3][0] = M[1][2] * M[2][3] - M[1][3] * M[2][2];
	Tmp[3][1] = M[0][2] * M[2][3] - M[0][3] * M[2][2];
	Tmp[3][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Det[0] = M[1][1] * Tmp[0][0] - M[2][1] * Tmp[0][1] + M[3][1] * Tmp[0][2];
	Det[1] = M[0][1] * Tmp[1][0] - M[2][1] * Tmp[1][1] + M[3][1] * Tmp[1][2];
	Det[2] = M[0][1] * Tmp[2][0] - M[1][1] * Tmp[2][1] + M[3][1] * Tmp[2][2];
	Det[3] = M[0][1] * Tmp[3][0] - M[1][1] * Tmp[3][1] + M[2][1] * Tmp[3][2];

	FFixed RDet = M[0][0] * Det[0] - M[1][0] * Det[1] + M[2][0] * Det[2] - M[3][0] * Det[3];

	Result[0][0] = Det[0] /  RDet;
	Result[0][1] = Det[1] / -RDet;
	Result[0][2] = Det[2] /  RDet;
	Result[0][3] = Det[3] / -RDet;
	Result[1][0] = (M[1][0] * Tmp[0][0] - M[2][0] * Tmp[0][1] + M[3][0] * Tmp[0][2]) / -RDet;
	Result[1][1] = (M[0][0] * Tmp[1][0] - M[2][0] * Tmp[1][1] + M[3][0] * Tmp[1][2]) /  RDet;
	Result[1][2] = (M[0][0] * Tmp[2][0] - M[1][0] * Tmp[2][1] + M[3][0] * Tmp[2][2]) / -RDet;
	Result[1][3] = (M[0][0] * Tmp[3][0] - M[1][0] * Tmp[3][1] + M[2][0] * Tmp[3][2]) /  RDet;
	Result[2][0] = (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) - M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) + M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])) /  RDet;
	Result[2][1] = (M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) - M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) + M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])) / -RDet;
	Result[2][2] = (M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) - M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) + M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])) /  RDet;
	Result[2][3] = (M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) - M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) + M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])) / -RDet;
	Result[3][0] = (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) - M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) + M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])) / -RDet;
	Result[3][1] = (M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) - M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) + M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])) /  RDet;
	Result[3][2] = (M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) - M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) + M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])) / -RDet;
	Result[3][3] = (M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) - M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) + M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])) /  RDet;

	return Return;
}

inline FFixedMatrix FFixedMatrix::TransposeAdjoint() const
{
	FFixedMatrix TA;

	TA.M[0][0] = M[1][1] * M[2][2] - M[1][2] * M[2][1];
	TA.M[0][1] = M[1][2] * M[2][0] - M[1][0] * M[2][2];
	TA.M[0][2] = M[1][0] * M[2][1] - M[1][1] * M[2][0];
	TA.M[0][3] = 0;

	TA.M[1][0] = M[2][1] * M[0][2] - M[2][2] * M[0][1];
	TA.M[1][1] = M[2][2] * M[0][0] - M[2][0] * M[0][2];
	TA.M[1][2] = M[2][0] * M[0][1] - M[2][1] * M[0][0];
	TA.M[1][3] = 0;

	TA.M[2][0] = M[0][1] * M[1][2] - M[0][2] * M[1][1];
	TA.M[2][1] = M[0][2] * M[1][0] - M[0][0] * M[1][2];
	TA.M[2][2] = M[0][0] * M[1][1] - M[0][1] * M[1][0];
	TA.M[2][3] = 0;

	TA.M[3][0] = 0;
	TA.M[3][1] = 0;
	TA.M[3][2] = 0;
	TA.M[3][3] = 1;

	return TA;
}

inline void FFixedMatrix::RemoveScaling()
{
	const FFixed SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const FFixed SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const FFixed SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
	FFixed Sqrt0 = FFixedMath::Sqrt(SquareSum0); Sqrt0 = Sqrt0 == 0 ? 1 : Sqrt0;
	FFixed Sqrt1 = FFixedMath::Sqrt(SquareSum0); Sqrt1 = Sqrt1 == 0 ? 1 : Sqrt1;
	FFixed Sqrt2 = FFixedMath::Sqrt(SquareSum0); Sqrt2 = Sqrt2 == 0 ? 1 : Sqrt2;
	M[0][0] /= Sqrt0;
	M[0][1] /= Sqrt0;
	M[0][2] /= Sqrt0;
	M[1][0] /= Sqrt1;
	M[1][1] /= Sqrt1;
	M[1][2] /= Sqrt1;
	M[2][0] /= Sqrt2;
	M[2][1] /= Sqrt2;
	M[2][2] /= Sqrt2;
}

inline FFixedVector FFixedMatrix::ExtractScaling()
{
	FFixedVector Scale3D(0, 0, 0);

	const FFixed SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const FFixed SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const FFixed SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);

	if (SquareSum0 == 0)
	{
		FFixed Scale0 = FFixedMath::Sqrt(SquareSum0);
		Scale3D[0] = Scale0;
		M[0][0] /= Scale0;
		M[0][1] /= Scale0;
		M[0][2] /= Scale0;
	}

	if (SquareSum1 == 0)
	{
		FFixed Scale1 = FFixedMath::Sqrt(SquareSum1);
		Scale3D[1] = Scale1;
		M[1][0] /= Scale1;
		M[1][1] /= Scale1;
		M[1][2] /= Scale1;
	}

	if (SquareSum2 == 0)
	{
		FFixed Scale2 = FFixedMath::Sqrt(SquareSum2);
		Scale3D[2] = Scale2;
		M[2][0] /= Scale2;
		M[2][1] /= Scale2;
		M[2][2] /= Scale2;
	}

	return Scale3D;
}

inline FFixedVector FFixedMatrix::GetScaleVector() const
{
	FFixedVector Scale3D(1, 1, 1);

	for (int32 i = 0; i < 3; i++)
	{
		const FFixed SquareSum = (M[i][0] * M[i][0]) + (M[i][1] * M[i][1]) + (M[i][2] * M[i][2]);
		Scale3D[i] = FFixedMath::Sqrt(SquareSum);
	}

	return Scale3D;
}

inline FFixedMatrix FFixedMatrix::RemoveTranslation() const
{
	FFixedMatrix Result = *this;
	Result.M[3][0] = 0;
	Result.M[3][1] = 0;
	Result.M[3][2] = 0;
	return Result;
}

inline FFixedMatrix FFixedMatrix::ConcatTranslation(const FFixedVector & Translation) const
{
	FFixedMatrix Result;

	Result.M[0] = M[0];
	Result.M[1] = M[1];
	Result.M[2] = M[2];

	Result.M[3][0] = M[3][0] + Translation[0];
	Result.M[3][1] = M[3][1] + Translation[1];
	Result.M[3][2] = M[3][2] + Translation[2];
	Result.M[3][3] = M[3][3];

	return Result;
}

inline void FFixedMatrix::ScaleTranslation(const FFixedVector & Scale3D)
{
	M[3][0] *= Scale3D.X;
	M[3][1] *= Scale3D.Y;
	M[3][2] *= Scale3D.Z;
}

inline FFixed FFixedMatrix::GetMaximumAxisScale() const
{
	const FFixed MaxRowScaleSquared = FFixedMath::Max(
		GetScaledAxis(EAxis::X).SizeSquared(),
		FFixedMath::Max(
			GetScaledAxis(EAxis::Y).SizeSquared(),
			GetScaledAxis(EAxis::Z).SizeSquared()
			)
		);
	return FFixedMath::Sqrt(MaxRowScaleSquared);
}

inline FFixedMatrix FFixedMatrix::ApplyScale(FFixed Scale) const
{
	FFixedMatrix ScaleMatrix(
		FFixedPlane(Scale, 0, 0, 0),
		FFixedPlane(0, Scale, 0, 0),
		FFixedPlane(0, 0, Scale, 0),
		FFixedPlane(0, 0, 0, 1)
	);
	return ScaleMatrix * (*this);
}

inline FFixedVector FFixedMatrix::GetScaledAxis(EAxis::Type InAxis) const
{
	switch (InAxis)
	{
	case EAxis::X:
		return FFixedVector(M[0][0], M[0][1], M[0][2]);

	case EAxis::Y:
		return FFixedVector(M[1][0], M[1][1], M[1][2]);

	case EAxis::Z:
		return FFixedVector(M[2][0], M[2][1], M[2][2]);

	default:
		ensure(0);
		return FFixedVector::ZeroVector;
	}
}

inline void FFixedMatrix::GetScaledAxes(FFixedVector & X, FFixedVector & Y, FFixedVector & Z) const
{
	X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
	Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
	Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
}

inline void FFixedMatrix::SetAxes(FFixedVector * Axis0, FFixedVector * Axis1, FFixedVector * Axis2, FFixedVector * Origin)
{
	if (Axis0 != nullptr)
	{
		M[0][0] = Axis0->X;
		M[0][1] = Axis0->Y;
		M[0][2] = Axis0->Z;
	}
	if (Axis1 != nullptr)
	{
		M[1][0] = Axis1->X;
		M[1][1] = Axis1->Y;
		M[1][2] = Axis1->Z;
	}
	if (Axis2 != nullptr)
	{
		M[2][0] = Axis2->X;
		M[2][1] = Axis2->Y;
		M[2][2] = Axis2->Z;
	}
	if (Origin != nullptr)
	{
		M[3][0] = Origin->X;
		M[3][1] = Origin->Y;
		M[3][2] = Origin->Z;
	}
}
