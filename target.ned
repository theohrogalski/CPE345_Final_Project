import inet.mobility.contract.IMobility;
import inet.common.neighborcache.NeighborCacheBase;

simple target{
@display("i=device/pc");

parameters:
    int id;
    int xpos=par("target%dx",id)
    int ypos=par("target%dx",id)

gates:
    input in[];
    output out[];
}
