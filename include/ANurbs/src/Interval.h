#pragma once

namespace ANurbs {

template <typename TScalar>
class Interval
{
public:
    using ScalarType = TScalar;

private:
    ScalarType m_t0;
    ScalarType m_t1;

public:
    Interval()
        : m_t0(0)
        , m_t1(0)
    {
    }

    Interval(
        const ScalarType t0,
        const ScalarType t1)
        : m_t0(t0)
        , m_t1(t1)
    {
    }

    ScalarType
    T0() const
    {
        return m_t0;
    }

    ScalarType
    T1() const
    {
        return m_t1;
    }

    ScalarType
    Min() const
    {
        return std::min(T0(), T1());
    }

    ScalarType
    Max() const
    {
        return std::max(T0(), T1());
    }

    ScalarType
    Delta() const
    {
        return T1() - T0();
    }

    ScalarType
    Length() const
    {
        return std::abs(Delta());
    }

    ScalarType
    NormalizedAt(
        const ScalarType t) const
    {
        return (t - T0()) / Length();
    }

    ScalarType
    ParameterAtNormalized(
        const ScalarType tNormalized) const
    {
        return T0() + Delta() * tNormalized;
    }

    static ScalarType
    Clamp(
        const ScalarType value,
        const ScalarType min,
        const ScalarType max)
    {
        if (value < min) {
            return min;
        }

        if (value > max) {
            return max;
        }

        return value;
    }

    static ScalarType
    Clamp01(
        const ScalarType value)
    {
        return Clamp(value, 0, 1);
    }

    ScalarType
    Clamp(
        const ScalarType value) const
    {
        return Clamp(value, Min(), Max());
    }
};

} // namespace ANurbs
