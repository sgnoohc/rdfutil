#include "rdfutil.h"

int main(int argc, char** argv)
{
    // Get the file names to process
    std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/230000/1A59D67A-325A-E742-BFFE-2FE5F69EB01B.root"};

    // Open the files into a RDataFrame
    RDataFrame rdf("Events", files);

    // Define a root node (all events with no filters and no modification)
    RNode root = rdf;

    // Select objects 
    RNode object_selections = root.Define("Muon_AnaLooseID", muon_2018LooseID, muon_2018LooseID_inputs)
                                  .Define("Electron_AnaLooseID", elec_2018LooseID, elec_2018LooseID_inputs)
                                  .Define("Jet_AnaLooseID", jet_2018ID, jet_2018ID_inputs);

    // Create branches based on selected objects
    RNode collection_building = object_selections;
    collection_building = TrimCollection(collection_building , "Muon"     , "Muon_AnaLooseID"     , "GoodMuon"     , Muon_properties);
    collection_building = TrimCollection(collection_building , "Electron" , "Electron_AnaLooseID" , "GoodElectron" , Electron_properties);
    collection_building = TrimCollection(collection_building , "Jet"      , "Jet_AnaLooseID"      , "PreORGoodJet" , Jet_properties);
    collection_building = MergeCollections(collection_building, "GoodMuon", "GoodElectron", "GoodLepton");
    collection_building = TrimCollectionByDeltaR(collection_building, "PreORGoodJet", "GoodLepton", "GoodJet", Jet_properties, 0.4f);

    // Print
    (*collection_building.Display({"GoodMuon_pt", "GoodElectron_pt", "GoodLepton_pt", "GoodLepton_pdgId", "nGoodJet", "nPreORGoodJet"}, 300)).Print();

    // Output
    collection_building.Snapshot("Events", "out.root", {".*Good.*"});
    TFile out("out.root", "RECREATE");
    out.Close();

}
