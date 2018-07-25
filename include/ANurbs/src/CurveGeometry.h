#pragma once

#include "CurveGeometryBase.h"
#include "Pointer.h"

#include <stdexcept>
#include <vector>

namespace ANurbs {

template <typename TScalar, int TDimension,
    typename TVector = Point<TScalar, TDimension>>
class CurveGeometry
    : public CurveGeometryBase<TScalar, TVector>
{
public:
    using CurveGeometryBaseType = CurveGeometryBase<TScalar, TVector>;
    using CurveGeometryType = CurveGeometry<TScalar, TDimension, TVector>;
    using typename CurveGeometryBaseType::KnotsType;
    using typename CurveGeometryBaseType::ScalarType;
    using typename CurveGeometryBaseType::VectorType;

protected:
    std::vector<VectorType> m_poles;
    std::vector<ScalarType> m_weights;

public:
    CurveGeometry(
        const int degree,
        const int nbPoles,
        const bool isRational)
        : CurveGeometryBaseType(degree, nbPoles)
        , m_poles(nbPoles)
        , m_weights(isRational ? nbPoles : 0)
    {
    }

    VectorType
    Pole(
        const int index) const override
    {
        return m_poles[index];
    }

    void
    SetPole(
        const int index,
        const VectorType& value) override
    {
        m_poles[index] = value;
    }

    bool
    IsRational() const override
    {
        return m_weights.size() != 0;
    }

    ScalarType
    Weight(
        const int index) const override
    {
        if (IsRational()) {
            return m_weights[index];
        } else {
            return 1;
        }
    }

    void
    SetWeight(
        const int index,
        const ScalarType value) override
    {
        if (IsRational()) {
            m_weights[index] = value;
        } else {
            throw std::invalid_argument("Geometry is not rational");
        }
    }

    Pointer<CurveGeometryType>
    Refined(
        const std::vector<ScalarType>& knotsToInsert) const
    {
        int nbKnotsToInsert = static_cast<int>(knotsToInsert.size());

        int a = Knots::UpperSpan(Degree(), Knots(), knotsToInsert.front());
        int b = Knots::UpperSpan(Degree(), Knots(), knotsToInsert.back());

        int nbPolesRefined = NbPoles() + nbKnotsToInsert;
        int nbKnotsRefined = NbKnots() + nbKnotsToInsert;

        auto refined = Create<CurveGeometryType>(Degree(), nbPolesRefined,
            false);

        for (int j = 0; j < a - Degree() + 2; j++) {
            refined->SetPole(j, Pole(j));
        }

        for (int j = b; j < NbPoles(); j++) {
            refined->SetPole(nbKnotsToInsert + j, Pole(j));
        }

        for (int j = 0; j < a + 1; j++) {
            refined->SetKnot(j, Knot(j));
        }

        for (int j = b + Degree(); j < NbKnots(); j++) {
            refined->SetKnot(nbKnotsToInsert + j, Knot(j));
        }

        int i = b + Degree() - 1;
        int k = b + Degree() + nbKnotsToInsert - 1;

        for (int j = nbKnotsToInsert - 1; j > -1; j--) {
            while (knotsToInsert[j] <= Knot(i) && i > a) {
                refined->SetPole(k - Degree(), Pole(i - Degree()));
                refined->SetKnot(k, Knot(i));
                k--;
                i--;
            }

            refined->SetPole(k - Degree(), refined->Pole(k - Degree() + 1));

            for (int l = 1; l < Degree() + 1; l++) {
                int index = k - Degree() + l;
                ScalarType alpha = refined->Knot(k + l) - knotsToInsert[j];

                if (abs(alpha) == 0) {
                    refined->SetPole(index, refined->Pole(index + 1));
                } else {
                    alpha = alpha / (refined->Knot(k + l) - Knot(i - Degree() + l));
                    refined->SetPole(index, refined->Pole(index) * alpha +
                        refined->Pole(index + 1) * (1 - alpha));
                }
            }

            refined->SetKnot(k, knotsToInsert[j]);

            k--;
        }

        return refined;
    }
};

using CurveGeometry1D = CurveGeometry<double, 1>;
using CurveGeometry2D = CurveGeometry<double, 2>;
using CurveGeometry3D = CurveGeometry<double, 3>;

} // namespace ANurbs
