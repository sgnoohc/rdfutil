// ROOT includes
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
using ROOT::RDataFrame;
using ROOT::RDF::RNode;
using ROOT::VecOps::RVec;

// ROOT includes
#include <TFile.h>
#include <TTree.h>

#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>
#include <cmath>
using LV = ROOT::Math::PtEtaPhiMVector;

// C++ includes
#include <iostream>
#include <string>

// ============================================================================
// Electron 2018 Loose Identification RDataFrame functional kernel
//
// Purpose
//   Compute a per electron boolean mask implementing a loose electron
//   preselection corresponding to Run 2 2018 analysis working points.
//   The mask is intended for fast object filtering and combinatorial
//   candidate building in RDataFrame workflows.
//
// Functional behavior
//   For each event the lambda evaluates each electron independently and
//   returns a boolean vector indicating which electrons satisfy the loose
//   identification requirements.
//
// Selection criteria per electron
//   abs(Electron_eta + Electron_deltaEtaSC) < 2.5
//   Electron_pt > 10
//   abs(Electron_dxy) < 0.05
//   abs(Electron_dz) < 0.10
//   abs(Electron_sip3d) < 8
//   Electron_miniPFRelIso_all < 0.4
//
// Inputs
//   Electron_pt
//   Electron_eta
//   Electron_deltaEtaSC
//   Electron_dxy
//   Electron_dz
//   Electron_sip3d
//   Electron_miniPFRelIso_all
//
// Output
//   RVec<bool> pass
//     pass[i] is true if electron i satisfies the loose ID
//     pass[i] is false otherwise
//
// ============================================================================
auto elec_2018LooseID = [](const RVec<float>& Electron_pt,
                           const RVec<float>& Electron_eta,
                           const RVec<float>& Electron_deltaEtaSC,
                           const RVec<float>& Electron_dxy,
                           const RVec<float>& Electron_dz,
                           const RVec<float>& Electron_sip3d,
                           const RVec<float>& Electron_miniPFRelIso_all)
{
    const auto n = Electron_pt.size();
    RVec<bool> pass(n, 0);
    for (size_t i = 0; i < n; ++i)
    {
        if (not (std::fabs(Electron_eta[i] + Electron_deltaEtaSC[i]) < 2.5f)) continue;
        if (not (Electron_pt[i] > 10.0f)) continue;
        if (not (std::fabs(Electron_dxy[i]) < 0.05f)) continue;
        if (not (std::fabs(Electron_dz[i]) < 0.1f)) continue;
        if (not (std::fabs(Electron_sip3d[i]) < 8.0f)) continue;
        if (not (Electron_miniPFRelIso_all[i] < 0.4f)) continue;
        pass[i] = 1;
    }
    return pass;
};

std::vector<std::string> elec_2018LooseID_inputs =
{
    "Electron_pt",
    "Electron_eta",
    "Electron_deltaEtaSC",
    "Electron_dxy",
    "Electron_dz",
    "Electron_sip3d",
    "Electron_miniPFRelIso_all"
};





// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------






// ============================================================================
// Muon 2018 Loose Identification RDataFrame functional kernel
//
// Purpose
//   Compute a per muon boolean mask implementing a loose muon preselection
//   corresponding to Run 2 2018 analysis working points. The mask is intended
//   for fast object filtering and combinatorial candidate building in
//   RDataFrame workflows.
//
// Functional behavior
//   For each event the lambda evaluates each muon independently and returns
//   a boolean vector indicating which muons satisfy the loose identification
//   requirements.
//
// Selection criteria per muon
//   Muon_pt > 10
//   abs(Muon_eta) < 2.4
//   abs(Muon_dxy) < 0.05
//   abs(Muon_dz) < 0.10
//   abs(Muon_sip3d) < 8
//   Muon_miniPFRelIso_all < 0.4
//   Muon_looseId is true
//
// Inputs
//   Muon_pt
//   Muon_eta
//   Muon_dxy
//   Muon_dz
//   Muon_sip3d
//   Muon_miniPFRelIso_all
//   Muon_looseId
//
// Output
//   RVec<bool> pass
//     pass[i] is true if muon i satisfies the loose ID
//     pass[i] is false otherwise
//
// ============================================================================
auto muon_2018LooseID = [](const RVec<float>& Muon_pt,
                           const RVec<float>& Muon_eta,
                           const RVec<float>& Muon_dxy,
                           const RVec<float>& Muon_dz,
                           const RVec<float>& Muon_sip3d,
                           const RVec<float>& Muon_miniPFRelIso_all,
                           const RVec<bool>& Muon_looseId)
{
    const auto n = Muon_pt.size();
    RVec<bool> pass(n, 0);
    for (size_t i = 0; i < n; ++i)
    {
        if (not (Muon_pt[i] > 10.0f)) continue;
        if (not (std::fabs(Muon_eta[i]) < 2.4f)) continue;
        if (not (std::fabs(Muon_dxy[i]) < 0.05f)) continue;
        if (not (std::fabs(Muon_dz[i]) < 0.1f)) continue;
        if (not (std::fabs(Muon_sip3d[i]) < 8.0f)) continue;
        if (not (Muon_miniPFRelIso_all[i] < 0.4f)) continue;
        if (not (Muon_looseId[i])) continue;
        pass[i] = 1;
    }
    return pass;
};

std::vector<std::string> muon_2018LooseID_inputs =
{
    "Muon_pt",
    "Muon_eta",
    "Muon_dxy",
    "Muon_dz",
    "Muon_sip3d",
    "Muon_miniPFRelIso_all",
    "Muon_looseId"
};





// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------





// ============================================================================
// Jet 2018 ID identification RDataFrame functional kernel
//
// Purpose
//   Compute a per jet boolean mask implementing a 2018 ID jet preselection.
//   The mask is intended for fast object filtering and combinatorial
//   candidate building in RDataFrame workflows.
//
// Functional behavior
//   For each event the lambda evaluates each jet independently and returns
//   a boolean vector indicating which jets satisfy the 2018 ID requirements.
//
// Selection criteria per jet
//   Jet_pt >= 30
//   abs(Jet_eta) <= 2.4
//   Jet_jetId >= 2
//
// Inputs
//   Jet_pt
//   Jet_eta
//   Jet_jetId
//
// Output
//   RVec<bool> pass
//     pass[j] is true if jet j satisfies the 2018 ID selection
//     pass[j] is false otherwise
//
// ============================================================================
auto jet_2018ID = [](const RVec<float>& Jet_pt,
                     const RVec<float>& Jet_eta,
                     const RVec<int>&   Jet_jetId)
{
    const auto n = Jet_pt.size();
    RVec<bool> pass(n, 0);

    for (size_t j = 0; j < n; ++j)
    {
        if (not (Jet_pt[j] >= 30.0f)) continue;
        if (not (std::fabs(Jet_eta[j]) <= 2.4f)) continue;
        if (not (Jet_jetId[j] >= 2)) continue;
        pass[j] = 1;
    }
    return pass;
};

std::vector<std::string> jet_2018ID_inputs =
{
    "Jet_pt",
    "Jet_eta",
    "Jet_jetId"
};





// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------





// ============================================================================
// Build a Good object collection from a NanoAOD style collection and a mask
//
// Purpose
//   Create a filtered view of an object collection by applying a previously
//   computed per object mask. The function defines new columns that follow
//   a NanoAOD like naming convention such as
//     GoodMuon_pt, GoodMuon_eta, GoodMuon_phi
//   and also defines the corresponding multiplicity counter
//     nGoodMuon
//
// Functional behavior
//   For each requested branch suffix S in branchSuffixes
//     outPrefix_S is defined as inPrefix_S[maskCol]
//   and a counter nOutPrefix is defined as the number of true entries in maskCol
//
// Inputs
//   inPrefix
//     Collection prefix in the input tree such as Muon, Electron, Jet
//
//   maskCol
//     Full name of the mask column such as Muon_2018LooseID
//
//   outPrefix
//     Output collection prefix such as GoodMuon, GoodElectron, GoodJet
//
//   branchSuffixes
//     List of branch suffixes to copy into the Good collection
//
// Outputs
//   For each suffix S
//     outPrefix_S
//
//   Counter
//     nOutPrefix
//
// ============================================================================
static RNode TrimCollection(RNode df,
                                const std::string& inPrefix,
                                const std::string& maskCol,
                                const std::string& outPrefix,
                                const std::vector<std::string>& branchSuffixes)
{
    const std::string nOutCol = "n" + outPrefix;

    RNode out = df;

    for (std::size_t i = 0; i < branchSuffixes.size(); ++i)
    {
        const std::string& suf = branchSuffixes[i];

        const std::string inCol  = inPrefix + "_" + suf;
        const std::string outCol = outPrefix + "_" + suf;

        const std::string expr = inCol + "[" + maskCol + "]";
        out = out.Define(outCol, expr);
    }

    const std::string countExpr = "(int)Sum(" + maskCol + ")";
    out = out.Define(nOutCol, countExpr);

    return out;
}


// ============================================================================
// Good muon collection construction
//
// Purpose
//   Build a GoodMuon collection from the Muon NanoAOD collection using the
//   Muon loose identification mask. The resulting collection contains
//   only muons passing the loose ID and defines a NanoAOD style multiplicity
//   counter nGoodMuon.
//
// Inputs
//   Muon_AnaLooseID
//
// Outputs
//   GoodMuon_pt
//   GoodMuon_eta
//   GoodMuon_phi
//   GoodMuon_charge
//   GoodMuon_dxy
//   GoodMuon_dz
//   GoodMuon_sip3d
//   GoodMuon_miniPFRelIso_all
//   nGoodMuon
//
// ============================================================================
std::vector<std::string> Muon_properties =
{
    "pt",
    "eta",
    "phi",
    "charge",
    "dxy",
    "dz",
    "sip3d",
    "miniPFRelIso_all"
};


// ============================================================================
// Good electron collection construction
//
// Purpose
//   Build a GoodElectron collection from the Electron NanoAOD collection using
//   the Electron loose identification mask. The resulting collection
//   contains only electrons passing the loose ID and defines a NanoAOD style
//   multiplicity counter nGoodElectron.
//
// Inputs
//   Electron_AnaLooseID
//
// Outputs
//   GoodElectron_pt
//   GoodElectron_eta
//   GoodElectron_phi
//   GoodElectron_charge
//   GoodElectron_dxy
//   GoodElectron_dz
//   GoodElectron_sip3d
//   GoodElectron_miniPFRelIso_all
//   nGoodElectron
//
// ============================================================================
std::vector<std::string> Electron_properties =
{
    "pt",
    "eta",
    "phi",
    "charge",
    "dxy",
    "dz",
    "sip3d",
    "miniPFRelIso_all"
};


// ============================================================================
// Good jet collection construction
//
// Purpose
//   Build a GoodJet collection from the Jet NanoAOD collection using a user
//   provided jet mask. The resulting collection contains only jets passing
//   the mask and defines a NanoAOD style multiplicity counter nGoodJet.
//
// Inputs
//   Jet_myMask
//
// Outputs
//   GoodJet_pt
//   GoodJet_eta
//   GoodJet_phi
//   GoodJet_mass
//   GoodJet_btagDeepFlavB
//   nGoodJet
//
// ============================================================================
std::vector<std::string> Jet_properties =
{
    "pt",
    "eta",
    "phi",
    "mass",
    "btagDeepFlavB"
};






// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------






// ============================================================================
// Build a combined GoodLepton collection from GoodMuon and GoodElectron
//
// Purpose
//   Merge previously built GoodMuon and GoodElectron collections into a single
//   GoodLepton collection, sort by descending pt, and attach a per lepton pdgId
//   consistent with charge and lepton flavor.
//
// Functional behavior
//   For each event
//     1) Concatenate GoodMuon and GoodElectron kinematics into one set
//     2) Build pdgId using charge and flavor
//        muon     pdgId = -13 if charge > 0 else +13
//        electron pdgId = -11 if charge > 0 else +11
//     3) Sort all leptons by pt in descending order
//     4) Reorder all output branches consistently using the sort indices
//     5) Define nGoodLepton following NanoAOD style naming
//
// Inputs
//   GoodMuon_pt          RVec<float>
//   GoodMuon_eta         RVec<float>
//   GoodMuon_phi         RVec<float>
//   GoodMuon_charge      RVec<int>
//   GoodElectron_pt      RVec<float>
//   GoodElectron_eta     RVec<float>
//   GoodElectron_phi     RVec<float>
//   GoodElectron_charge  RVec<int>
//
// Outputs
//   GoodLepton_pt        RVec<float>
//   GoodLepton_eta       RVec<float>
//   GoodLepton_phi       RVec<float>
//   GoodLepton_charge    RVec<int>
//   GoodLepton_pdgId     RVec<int>
//   GoodLepton_srcIdx    RVec<int>
//   nGoodLepton          int
//
// Notes
//   This helper assumes the GoodMuon and GoodElectron collections already exist
//   and contain only leptons passing your intended IDs.
// ============================================================================
static RNode MergeCollections(RNode df,
                             const std::string& goodMuonPrefix,
                             const std::string& goodElectronPrefix,
                             const std::string& outPrefix)
{
    const std::string mu_pt     = goodMuonPrefix + "_pt";
    const std::string mu_eta    = goodMuonPrefix + "_eta";
    const std::string mu_phi    = goodMuonPrefix + "_phi";
    const std::string mu_charge = goodMuonPrefix + "_charge";

    const std::string el_pt     = goodElectronPrefix + "_pt";
    const std::string el_eta    = goodElectronPrefix + "_eta";
    const std::string el_phi    = goodElectronPrefix + "_phi";
    const std::string el_charge = goodElectronPrefix + "_charge";

    const std::string out_pt     = outPrefix + "_pt";
    const std::string out_eta    = outPrefix + "_eta";
    const std::string out_phi    = outPrefix + "_phi";
    const std::string out_charge = outPrefix + "_charge";
    const std::string out_pdgId  = outPrefix + "_pdgId";
    const std::string out_srcIdx = outPrefix + "_srcIdx";
    const std::string out_n      = "n" + outPrefix;

    const std::string carrier = outPrefix + "_carrier";

    struct GoodLeptonCarrier {
        RVec<float> pt;
        RVec<float> eta;
        RVec<float> phi;
        RVec<int>   charge;
        RVec<int>   pdgId;
        RVec<int>   srcIdx;
    };

    RNode df1 =
        df.Define(
            carrier,
            [](const RVec<float>& Mu_pt,
               const RVec<float>& Mu_eta,
               const RVec<float>& Mu_phi,
               const RVec<int>&   Mu_charge,
               const RVec<float>& El_pt,
               const RVec<float>& El_eta,
               const RVec<float>& El_phi,
               const RVec<int>&   El_charge) -> GoodLeptonCarrier
            {
                const size_t nmu = Mu_pt.size();
                const size_t nel = El_pt.size();
                const size_t n   = nmu + nel;

                GoodLeptonCarrier out;
                out.pt.resize(n);
                out.eta.resize(n);
                out.phi.resize(n);
                out.charge.resize(n);
                out.pdgId.resize(n);
                out.srcIdx.resize(n);

                size_t k = 0;

                for (size_t i = 0; i < nmu; ++i)
                {
                    out.pt[k]     = Mu_pt[i];
                    out.eta[k]    = Mu_eta[i];
                    out.phi[k]    = Mu_phi[i];
                    out.charge[k] = Mu_charge[i];
                    out.srcIdx[k] = i;

                    const int q = (Mu_charge[i] > 0.0f) ? +1 : -1;
                    out.pdgId[k] = (q > 0) ? -13 : +13;

                    ++k;
                }

                for (size_t i = 0; i < nel; ++i)
                {
                    out.pt[k]     = El_pt[i];
                    out.eta[k]    = El_eta[i];
                    out.phi[k]    = El_phi[i];
                    out.charge[k] = El_charge[i];
                    out.srcIdx[k] = i;

                    const int q = (El_charge[i] > 0.0f) ? +1 : -1;
                    out.pdgId[k] = (q > 0) ? -11 : +11;

                    ++k;
                }

                std::vector<size_t> idx(n);
                for (size_t i = 0; i < n; ++i) idx[i] = i;

                std::sort(idx.begin(), idx.end(),
                          [&out](size_t a, size_t b) { return out.pt[a] > out.pt[b]; });

                GoodLeptonCarrier sorted;
                sorted.pt.resize(n);
                sorted.eta.resize(n);
                sorted.phi.resize(n);
                sorted.charge.resize(n);
                sorted.pdgId.resize(n);
                sorted.srcIdx.resize(n);

                for (size_t i = 0; i < n; ++i)
                {
                    const size_t s = idx[i];
                    sorted.pt[i]     = out.pt[s];
                    sorted.eta[i]    = out.eta[s];
                    sorted.phi[i]    = out.phi[s];
                    sorted.charge[i] = out.charge[s];
                    sorted.pdgId[i]  = out.pdgId[s];
                    sorted.srcIdx[i] = out.srcIdx[s];
                }

                return sorted;
            },
            {mu_pt, mu_eta, mu_phi, mu_charge,
             el_pt, el_eta, el_phi, el_charge});

    RNode df2 =
        df1.Define(out_pt   , [](const GoodLeptonCarrier& c) -> RVec<float> { return c.pt; }, {carrier})
          .Define(out_eta   , [](const GoodLeptonCarrier& c) -> RVec<float> { return c.eta; }, {carrier})
          .Define(out_phi   , [](const GoodLeptonCarrier& c) -> RVec<float> { return c.phi; }, {carrier})
          .Define(out_charge, [](const GoodLeptonCarrier& c) -> RVec<int> { return c.charge; }, {carrier})
          .Define(out_pdgId , [](const GoodLeptonCarrier& c) -> RVec<int> { return c.pdgId; }, {carrier})
          .Define(out_srcIdx, [](const GoodLeptonCarrier& c) -> RVec<int> { return c.srcIdx; }, {carrier})
          .Define(out_n     , [](const GoodLeptonCarrier& c) -> int { return static_cast<int>(c.pt.size()); }, {carrier});

    return df2;
}





// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------





// ============================================================================
// Overlap removal by deltaR against a reference collection
//
// Purpose
//   Remove objects from a source collection if they overlap with any object in
//   a reference collection within a configurable deltaR threshold. The output
//   is a new collection containing only non overlapping objects and a NanoAOD
//   style multiplicity counter.
//
// Functional behavior
//   For each event, build a per object overlap mask for the source collection.
//   A source object is rejected if there exists at least one reference object
//   with deltaR(source, reference) < dR. The mask is then used to project the
//   requested source branches into a new OR collection.
//
// Inputs
//   Source collection columns
//     srcPrefix_pt
//     srcPrefix_eta
//     srcPrefix_phi
//   Reference collection columns
//     refPrefix_eta
//     refPrefix_phi
//
// Outputs
//   A mask column
//     outPrefix_ORmask
//   A projected overlap removed collection
//     outPrefix_suffix for each suffix in srcBranchSuffixes
//   A multiplicity counter
//     nOutPrefix
//
// Notes
//   This helper is intended to be used after you have already created Good
//   collections. It is generic for jets, leptons, photons, etc, as long as the
//   collections use NanoAOD style eta and phi columns.
// ============================================================================
static RNode TrimCollectionByDeltaR(RNode df,
                               const std::string& srcPrefix,
                               const std::string& refPrefix,
                               const std::string& outPrefix,
                               const std::vector<std::string>& srcBranchSuffixes,
                               const float dR)
{
    const std::string srcEta = srcPrefix + "_eta";
    const std::string srcPhi = srcPrefix + "_phi";

    const std::string refEta = refPrefix + "_eta";
    const std::string refPhi = refPrefix + "_phi";

    const std::string maskCol = outPrefix + "_ORmask";

    const std::string nOutCol = "n" + outPrefix;

    // Build overlap mask for the source collection
    RNode df1 =
        df.Define(
            maskCol,
            [dR](const RVec<float>& sEta,
                 const RVec<float>& sPhi,
                 const RVec<float>& rEta,
                 const RVec<float>& rPhi) -> RVec<bool>
            {
                const size_t ns = sEta.size();
                const size_t nr = rEta.size();

                RVec<bool> keep(ns, true);

                for (size_t i = 0; i < ns; ++i)
                {
                    bool overlaps = false;

                    for (size_t j = 0; j < nr; ++j)
                    {
                        LV p4_0(1, sEta[i], sPhi[i], 0);
                        LV p4_1(1, rEta[j], rPhi[j], 0);

                        float this_dr = ROOT::Math::VectorUtil::DeltaR(p4_0, p4_1);

                        if (this_dr < dR)
                        {
                            overlaps = true;
                            break;
                        }
                    }

                    if (overlaps) keep[i] = false;
                }

                return keep;
            },
            {srcEta, srcPhi, refEta, refPhi});

    // Project masked source branches into the overlap removed output collection
    RNode out = df1;

    for (std::size_t i = 0; i < srcBranchSuffixes.size(); ++i)
    {
        const std::string& suf = srcBranchSuffixes[i];

        const std::string inCol  = srcPrefix + "_" + suf;
        const std::string outCol = outPrefix + "_" + suf;

        const std::string expr = inCol + "[" + maskCol + "]";
        out = out.Define(outCol, expr);
    }

    // Define NanoAOD style multiplicity counter for the OR collection
    const std::string countExpr = "(int)Sum(" + maskCol + ")";
    out = out.Define(nOutCol, countExpr);

    return out;
}


// ============================================================================
// Example usage: overlap remove GoodJet against GoodLepton
//
// Purpose
//   Remove jets overlapping with any lepton and build a new OR jet collection.
//
// Inputs
//   Source collection
//     GoodJet_eta, GoodJet_phi, and the GoodJet branches in Jet_properties
//   Reference collection
//     GoodLepton_eta, GoodLepton_phi
//
// Outputs
//   ORGoodJet_eta, ORGoodJet_phi, and other ORGoodJet branches in Jet_properties
//   nORGoodJet
//
// ============================================================================





// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------
// -------------- ============ --------------------- ============ ---------------






// ============================================================================
// Z candidate selection from a lepton collection (opposite sign, closest to Z)
//
// Purpose
//   Build an opposite sign lepton pair candidate whose invariant mass is closest
//   to the Z boson mass. The selection is performed within a single lepton
//   collection (Muon or Electron) and restricted to leptons passing a provided
//   per lepton ID mask.
//
// Functional behavior
//   For each event, scan all lepton pairs i < j that pass:
//     1) Lepton_ID[i] and Lepton_ID[j] are true
//     2) Lepton_charge[i] * Lepton_charge[j] < 0 (opposite sign)
//   For all such pairs, compute mll and choose the pair with minimum
//   abs(mll - mZ). If no valid pair exists, indices are set to -1 and mll to -1.
//
// Inputs
//   Collection dependent column names are built from the collection prefix.
//     prefix + "_pt"     (RVec<float>)
//     prefix + "_eta"    (RVec<float>)
//     prefix + "_phi"    (RVec<float>)
//     prefix + "_charge" (RVec<float> or RVec<int>, any numeric type works)
//   In addition, the caller provides the name of an ID mask column:
//     idMaskCol          (RVec<bool>)
//
// Outputs
//   Three scalar columns are defined on the returned node:
//     outPrefix + "_Zidx0"    (int)
//     outPrefix + "_Zidx1"    (int)
//     outPrefix + "_MindMzMll" (float)
//
// Notes
//   This helper is intended to be called separately for Muon and Electron.
//   Output column names are prefixed to avoid collisions when multiple
//   collections are processed in the same analysis.
// ============================================================================

struct ZCandResult {
    int idx0;
    int idx1;
    float mll;
};

static RNode SelectZCandidate(RNode df,
                             const std::string& collectionPrefix,
                             const std::string& idMaskCol,
                             const std::string& outPrefix)
{
    const std::string ptCol     = collectionPrefix + "_pt";
    const std::string etaCol    = collectionPrefix + "_eta";
    const std::string phiCol    = collectionPrefix + "_phi";
    const std::string chargeCol = collectionPrefix + "_charge";

    const std::string carrierCol = outPrefix + "_ZCand";

    const std::string outIdx0 = outPrefix + "_Zidx0";
    const std::string outIdx1 = outPrefix + "_Zidx1";
    const std::string outMll  = outPrefix + "_MindMzMll";

    const ROOT::RDF::ColumnNames_t inputs = {ptCol, etaCol, phiCol, chargeCol, idMaskCol};

    RNode df1 =
        df.Define(
            carrierCol,
            [](const RVec<float>& Lepton_pt,
               const RVec<float>& Lepton_eta,
               const RVec<float>& Lepton_phi,
               const RVec<float>& Lepton_charge,
               const RVec<bool>&  Lepton_ID) -> ZCandResult
            {
                const std::size_t n = Lepton_pt.size();

                ZCandResult out;
                out.idx0 = -1;
                out.idx1 = -1;
                out.mll  = -1.0f;

                float best_dm = 1.0e30f;
                const float mZ = 91.188f;

                for (std::size_t i = 0; i < n; ++i) {
                    for (std::size_t j = i + 1; j < n; ++j) {
                        if (!(Lepton_ID[i] && Lepton_ID[j])) continue;
                        if (!((Lepton_charge[i] * Lepton_charge[j]) < 0.0f)) continue;

                        const LV lep0(Lepton_pt[i], Lepton_eta[i], Lepton_phi[i], 0.0);
                        const LV lep1(Lepton_pt[j], Lepton_eta[j], Lepton_phi[j], 0.0);

                        const float mll = static_cast<float>((lep0 + lep1).M());
                        const float dm  = std::abs(mll - mZ);

                        if (dm < best_dm) {
                            best_dm  = dm;
                            out.idx0 = static_cast<int>(i);
                            out.idx1 = static_cast<int>(j);
                            out.mll  = mll;
                        }
                    }
                }
                return out;
            },
            inputs);

    RNode df2 =
        df1.Define(outIdx0, [](const ZCandResult& z) -> int  { return z.idx0; }, {carrierCol})
           .Define(outIdx1, [](const ZCandResult& z) -> int  { return z.idx1; }, {carrierCol})
           .Define(outMll , [](const ZCandResult& z) -> float { return z.mll; }, {carrierCol});

    return df2;
}

// Thin wrappers to keep the main analysis code simple and collision free.
// Adjust the ID mask column names to match your analysis definitions.

static RNode SelectZCandidateMuon(RNode df, const std::string& muonIdMaskCol)
{
    return SelectZCandidate(df, "Muon", muonIdMaskCol, "MuonZPair");
}

static RNode SelectZCandidateElectron(RNode df, const std::string& eleIdMaskCol)
{
    return SelectZCandidate(df, "Electron", eleIdMaskCol, "ElectronZPair");
}


