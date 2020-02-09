#include "calctrack.h"
#include "math.h"

CalcTrack::CalcTrack()
{
    m_Ls = m_rLA * 2.0;
    m_Lp = sqrt((m_xk - m_xn) * (m_xk - m_xn) + (m_yk - m_yn) * (m_yk - m_yn));
    m_Sp = m_Lp;
    if(m_Lp / m_Sp > 3 && m_Lp / m_Ls > 3) {

    }
}

void CalcTrack::polosa()
{
    m_isError = false;
    int j, i;

}
