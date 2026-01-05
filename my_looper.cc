#include "rdfutil.h"

int main(int argc, char** argv)
{
    // Get the file names to process
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/230000/1A59D67A-325A-E742-BFFE-2FE5F69EB01B.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1_ext1-v2/2500000/2A0EB266-E607-8646-971D-45D5F962C147.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/WWZJetsTo4L2Nu_4F_TuneCP5_13TeV-amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v2/40000/8718B61F-4D6B-6B4C-AA07-A29ADBDC7FD8.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/TTZToLL_TuneCP5_13TeV_amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v3/2830000/91815FD7-FD43-7444-92BB-D0CB29A30212.root"};
    // std::vector<std::string> files = {"/blue/avery/p.chang/nanoaod/data/Run2018D/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v2/2430000/8BC1280C-A72D-1B4A-BE4D-9BFDA7E4C862.root"};
    // std::vector<std::string> files =
    // {
    //     // "/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/TTZToLL_TuneCP5_13TeV_amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v3/2830000/91815FD7-FD43-7444-92BB-D0CB29A30212.root",
    //     // "/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/GluGluHToZZTo4L_M125_TuneCP5_13TeV_powheg2_JHUGenV7011_pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v1/120000/3ED05633-EBB7-4A44-8F9D-CD956490BCFD.root",
    //     // "/blue/avery/p.chang/nanoaod/mc/RunIISummer20UL18NanoAODv9/TTZToLL_TuneCP5_13TeV_amcatnlo-pythia8/NANOAODSIM/106X_upgrade2018_realistic_v16_L1v1-v3/2830000/91815FD7-FD43-7444-92BB-D0CB29A30212.root",
    //     // "/blue/avery/p.chang/nanoaod/data/Run2018D/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v2/2430000/8BC1280C-A72D-1B4A-BE4D-9BFDA7E4C862.root",
    //     "/blue/avery/p.chang/nanoaod/data/Run2018D/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v2/2430000/*.root",
    // };

    std::vector<std::string> files = RdfUtil::CollectRootFiles("/blue/avery/p.chang/nanoaod/data/Run2018D/DoubleMuon/NANOAOD/UL2018_MiniAODv2_NanoAODv9-v2/2430000/");

    const bool isDataJob = RdfUtil::IsDataFromFirstFile(files.at(0));

    // Open the files into a RDataFrame
    RDataFrame rdf("Events", files);

    // Define a root node (all events with no filters and no modification)
    RNode df = rdf;

    ROOT::RDF::Experimental::AddProgressBar(rdf);

    RdfUtil::PrintSampleMetaDataRegistry("samples.csv");
    df = RdfUtil::BuildSampleMetaData(df, "samples.csv");

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

    df = df.Filter("nGoodLepton==4", "N_{lep}=4");
    df = RdfUtil::PairCollection(df, "GoodLepton", "GoodLepton", "GoodZ", RdfUtil::selAbsMassDiff(91.1876), RdfUtil::MinimizeScore);
    df = RdfUtil::TrimCollection(df, "GoodLepton", "GoodLepton_goodZIdx<0", "GoodLepNoZ", GoodLepton_properties);
    df = RdfUtil::PairCollection(df, "GoodLepNoZ", "GoodLepNoZ", "GoodZ2", RdfUtil::selAbsMassDiff(91.1876), RdfUtil::MinimizeScore);
    df = RdfUtil::PairCollection(df, "GoodZ", "GoodZ2", "GoodH", RdfUtil::selAbsMassDiff(125), RdfUtil::MinimizeScore);

    if (not isDataJob)
    {
        df = df.Define("GenPart_save", gen_mask, gen_mask_inputs)
               .Define("GenPart_lepOrigin", lep_origin_mask, lep_origin_mask_inputs)
               ;
        df = RdfUtil::TrimCollection(df, "GenPart"  , "GenPart_save"        , "GoodGenPart"  , GenPart_properties);
        df = RdfUtil::TrimCollection(df, "GenPart"  , "GenPart_status==1&&(abs(GenPart_pdgId)==11||abs(GenPart_pdgId)==13)", "GoodGenLep", GenPart_properties);
        df = RdfUtil::MatchCollection(df, "GoodLepton", "GoodGenLep", 0.1f);
        df = RdfUtil::PairCollection(df, "GoodGenLep", "GoodGenLep", "GoodGenZ", RdfUtil::selAbsMassDiff(91.1876), RdfUtil::MinimizeScore);
        df = RdfUtil::TrimCollection(df, "GoodGenLep", "GoodGenLep_goodGenZIdx<0", "GoodGenLepNoZ", GenPart_properties);
        df = RdfUtil::PairCollection(df, "GoodGenLepNoZ", "GoodGenLepNoZ", "GoodGenZ2", RdfUtil::selAbsMassDiff(91.1876), RdfUtil::MinimizeScore);
        df = RdfUtil::PairCollection(df, "GoodGenZ", "GoodGenZ2", "GoodGenH", RdfUtil::selAbsMassDiff(125), RdfUtil::MinimizeScore);
    }

    // df = RdfUtil::TrioCollection(df, "GoodJet", "GoodJet", "GoodJet", "GoodTop", RdfUtil::selTrioAbsMassDiff(172.5), RdfUtil::MinimizeScore);

    // Print
    // (*df.Display({"GoodLepton_goodGenLepIdx", "GoodLepton_pt", "GoodGenLep_lepOrigin", "GoodGenLep_pt"}, 23, 200)).Print();
    // (*df.Display({"GoodLepton_eta", "GoodLepton_phi", "GoodGenLep_eta", "GoodGenLep_phi", "GoodGenLep_pdgId", "GoodGenLep_pt"}, 23, 200)).Print();
    // (*df.Display({"GoodTop_goodJetIdx1", "GoodTop_goodJetIdx2", "GoodTop_goodJetIdx3"}, 23, 200)).Print();
    // (*df.Display({"GoodZ_goodLeptonIdx1", "GoodZ_goodLeptonIdx2", "GoodLepton_goodGenLepIdx", "GoodGenLep_lepOrigin"}, 23, 200)).Print();
    // (*df.Display({"GoodZ_goodLeptonIdx1", "GoodZ_goodLeptonIdx2", "GoodLepton_goodGenLepIdx", "GoodGenLep_lepOrigin"}, 23, 200)).Print();

    // Output
    auto cols_to_write = RdfUtil::SelectColumnNames(df, {"Good*", "nGood*", "Sample*", "Electron*", "Muon*"});
    df.Snapshot("Events", "out.root", cols_to_write);

}
