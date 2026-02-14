#include "rdfutil.h"

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.root> <output.root> [nthreads]" << std::endl;
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string output_file_name = argv[2];
    int nthreads = (argc >= 4) ? std::atoi(argv[3]) : 4;

    if (nthreads > 1)
        ROOT::EnableImplicitMT(nthreads);
    else
        ROOT::DisableImplicitMT();

    const bool isDataJob = true;

    std::vector<std::string> inputfiles;
    std::ifstream infile(input_file_name); // argv[1] = files.txt
    std::string line;

    while (std::getline(infile, line))
    {
        // Find first non-space/tab character
        auto first = line.find_first_not_of(" \t");

        // Skip empty or whitespace-only lines
        if (first == std::string::npos) continue;

        // Skip comments (after whitespace)
        if (line[first] == '#') continue;

        // Store trimmed line (optional but cleaner)
        inputfiles.push_back(line.substr(first));
    }

    if (inputfiles.empty())
    {
        std::cerr << "No input files found in " << input_file_name << std::endl;
        return 1;
    }

    RDataFrame rdf("Events", inputfiles);

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

    // Trigger
    df = RdfUtil::DefineTriggers(df, {
        {"Trig_passDilepton", RdfUtil::DileptonTriggers},
    });

    df = df.Filter("nGoodLepton==4", "N_{lep}=4");

    // Output
    auto cols_to_write = RdfUtil::SelectColumnNames(df, {"GoodLepton*", "nGoodLepton", "GoodJet*", "nGoodJet", "MET*", "Trig_*", "genWeight", "run", "lumi*", "event"});
    df.Snapshot("Events", output_file_name, cols_to_write);

    return 0;

}
