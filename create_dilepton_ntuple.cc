#include "rdfutil.h"

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.root> <output.root>" << std::endl;
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string output_file_name = argv[2];

    ROOT::EnableImplicitMT(32);
    // ROOT::DisableImplicitMT(); // or just don't EnableImplicitMT()

    const bool isDataJob = true;

    // RDataFrame rdf("Events", "root://cmsio2.rc.ufl.edu//store/data/Run2018B/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v1/270000/098ABF75-8D93-1B45-84A1-981613A831E7.root");
    // RDataFrame rdf("Events", "/blue/avery/p.chang/nanoaod/data/Run2018D/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v2/2430000/*.root");
    // RDataFrame rdf("Events", "/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/40000/*.root");
    RDataFrame rdf("Events", input_file_name + "/*.root");

    // Defioe a root node (all events with no filters and no modification)
    RNode df = rdf;

    ROOT::RDF::Experimental::AddProgressBar(df);

    // Select objects 
    df = df.Define("Muon_anaID", muon_2018ID, muon_2018ID_inputs)
           .Define("Electron_anaID", elec_2018ID, elec_2018ID_inputs)
           .Define("Jet_anaLooseID", jet_2018ID, jet_2018ID_inputs)
           ;

    // Create branches based on selected objects
    df = RdfUtil::TrimCollection(df, "Muon"     , "Muon_anaID>=1"     , "GoodLooseMuon"     , Muon_properties);
    df = RdfUtil::TrimCollection(df, "Electron" , "Electron_anaID>=1" , "GoodLooseElectron" , Electron_properties);
    df = RdfUtil::TrimCollection(df, "Muon"     , "Muon_anaID>=2"     , "GoodMuon"     , Muon_properties);
    df = RdfUtil::TrimCollection(df, "Electron" , "Electron_anaID>=2" , "GoodElectron" , Electron_properties);
    df = RdfUtil::TrimCollection(df, "Jet"      , "Jet_anaLooseID"      , "PreORGoodJet" , Jet_properties);
    df = RdfUtil::MergeCollections(df, "GoodMuon", "GoodElectron", "GoodLepton");
    df = RdfUtil::TrimCollectionByDeltaR(df, "PreORGoodJet", "GoodLepton", "GoodJet", Jet_properties, 0.4f);

    df = df.Filter("nGoodLepton==2", "N_{lep}=2");
    df = df.Filter("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8==1", "HLT_DoubleMuon");

    // Output
    auto cols_to_write = RdfUtil::SelectColumnNames(df, {"GoodLepton*", "nGoodLepton", "GoodJet*", "nGoodJet", "MET*", "genWeight", "run", "lumi*", "event"});
    df.Snapshot("Events", output_file_name, cols_to_write);

    return 0;

}
