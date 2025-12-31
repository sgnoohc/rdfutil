#include "rdfutil.h"

int main(int argc, char** argv)
{
    // Get the file names to process
    std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/230000/1A59D67A-325A-E742-BFFE-2FE5F69EB01B.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1_ext1-v2/2500000/2A0EB266-E607-8646-971D-45D5F962C147.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/WWZJetsTo4L2Nu_4F_TuneCP5_13TeV-amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/40000/8718B61F-4D6B-6B4C-AA07-A29ADBDC7FD8.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/TTZToLL_TuneCP5_13TeV_amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v3/2830000/91815FD7-FD43-7444-92BB-D0CB29A30212.root"};

    // Open the files into a RDataFrame
    RDataFrame rdf("Events", files);

    // Define a root node (all events with no filters and no modification)
    RNode root = rdf;

    // Select objects 
    RNode object_selections = root.Define("Muon_anaLooseID", muon_2018LooseID, muon_2018LooseID_inputs)
                                  .Define("Electron_anaLooseID", elec_2018LooseID, elec_2018LooseID_inputs)
                                  .Define("Jet_anaLooseID", jet_2018ID, jet_2018ID_inputs)
                                  .Define("GenPart_save", gen_mask, gen_mask_inputs);

    // Create branches based on selected objects
    RNode collection_building = object_selections;
    collection_building = RdfUtil::TrimCollection(collection_building , "Muon"     , "Muon_anaLooseID"     , "GoodMuon"     , Muon_properties);
    collection_building = RdfUtil::TrimCollection(collection_building , "Electron" , "Electron_anaLooseID" , "GoodElectron" , Electron_properties);
    collection_building = RdfUtil::TrimCollection(collection_building , "Jet"      , "Jet_anaLooseID"      , "PreORGoodJet" , Jet_properties);
    collection_building = RdfUtil::TrimCollection(collection_building , "GenPart"  , "GenPart_save"        , "GoodGenPart"  , GenPart_properties);
    collection_building = RdfUtil::MergeCollections(collection_building, "GoodMuon", "GoodElectron", "GoodLepton");
    collection_building = RdfUtil::TrimCollectionByDeltaR(collection_building, "PreORGoodJet", "GoodLepton", "GoodJet", Jet_properties, 0.4f);
    collection_building = RdfUtil::MatchCollection(collection_building, "GoodLepton", "GoodGenPart", 0.1f);


    // Print
    // (*collection_building.Display({"GoodGenPart_pdgId", "GoodGenPart_status", "GoodGenPart_statusFlags", "GoodGenPart_pt"}, 9, 200)).Print();
    (*collection_building.Display({"GoodGenPart_pdgId", "GoodGenPart_status", "GoodGenPart_statusFlags", "GoodGenPart_pt"}, 23, 200)).Print();
    // (*collection_building.Display({"GoodLepton_goodGenPartIdx", "GoodLepton_goodGenPartDr", "GoodLepton_pt", "GoodGenPart_pt"}, 23, 200)).Print();

    // Output
    auto cols_to_write = RdfUtil::SelectColumnNames(collection_building, {"Good*"});
    collection_building.Snapshot("Events", "out.root", cols_to_write);

}
