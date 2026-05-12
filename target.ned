import inet.mobility.contract.IMobility;
import inet.common.neighborcache.NeighborCacheBase;

simple target{
@display("i=device/pc");

parameters:
    int id;
    string xpos_targ= "target" + to_string(this->id) +"x";
    string xpos_targ= "target" + to_string(this->id) +"y";
    int xpos=par("target%dx",id)
    int ypos=par("target%dx",id)

gates:
    input in[];
    output out[];
submodules:
    targ0: target{}
    targ1:target{}
}
