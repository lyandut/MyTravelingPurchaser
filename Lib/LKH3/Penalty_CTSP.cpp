#include "LKH.h"
#include "Segment.h"

GainType Penalty_CTSP()
{
    static thread_local Node *StartRoute = 0;
    Node *N, *N1, *N2, *CurrentRoute;
    GainType P = 0;
    int Forward;

    N1 = Depot;
    do ; while ((N1 = SUCC(N1))->DepotId == 0);
    N2 = Depot;
    do ; while ((N2 = PREDD(N2))->DepotId == 0);
    Forward = N1->DepotId < N2->DepotId;

    if (!StartRoute)
        StartRoute = Depot;
    N = StartRoute;
    do {
        CurrentRoute = N;
        do {
            if (N->Color != 0 && N->Color != CurrentRoute->DepotId)
                P++;
        } while ((N = Forward ? SUCC(N) : PREDD(N))->DepotId == 0);
        if (P > CurrentPenalty ||
            (P == CurrentPenalty && CurrentGain <= 0)) {
            StartRoute = CurrentRoute;
            return CurrentPenalty + (CurrentGain > 0);
        }
    } while (N != StartRoute);
    return P;
}
