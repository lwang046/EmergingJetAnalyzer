// -*- C++ -*-
//
// Package:    EmergingAnalysis/WJetFilter
// Class:      WJetFilter
// 
/**\class WJetFilter WJetFilter.cc EmergingAnalysis/WJetFilter/plugins/WJetFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Young Ho Shin
//         Created:  Mon, 10 Aug 2015 23:22:24 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TVector3.h>
#include <TMath.h>
#include <Math/VectorUtil.h>

// For creating/writing histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

// Data formats
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// Namespace shorthands
using std::string;
using std::vector;

//
// class declaration
//

class WJetFilter : public edm::EDFilter {
  public:
    explicit WJetFilter(const edm::ParameterSet&);
    ~WJetFilter();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // ----------member data ---------------------------
    // Inputs
    string alias_; // Alias suffix for all products
    bool isData_;
    // Retrieve once per event
    edm::EDGetTokenT< pat::MuonCollection > muonCollectionToken_;
    edm::EDGetTokenT< pat::ElectronCollection > electronCollectionToken_;
    edm::EDGetTokenT< pat::JetCollection > jetCollectionToken_;
    edm::EDGetTokenT< pat::METCollection > metCollectionToken_;
    double minPtMuon_;
    double minPtElectron_;
    double minPtMET_;
    double minMt_;
    double maxMt_;
    double minDeltaR_;
    double maxDeltaPhi_;
    double minPtSelectedJet_;
    double maxPtAdditionalJets_;
    string electronID_;

    // Outputs
    std::unordered_map<string, TH1*> histoMap1D_;
    std::unordered_map<string, TH2*> histoMap2D_;

    TTree* outputTree;
    struct outputClass {
      public:
        int run;
        int lumi;
        int event;
        double gen_weight;
        double met_pt;
        double met_phi;
        double lepton_pt;
        double lepton_eta;
        double lepton_phi;
        double lepton_relIso;
        double mT_e;
        double mT_u;
        double WpT_e;
        double WpT_u;
        vector<double> jets_pt;
        vector<double> jets_eta;
        vector<double> jets_phi;
    };
    outputClass output;
};

//
// constants, enums and typedefs
//
/// Lorentz vector
typedef reco::Candidate::LorentzVector RecoLorentzVector;
/// Lorentz vector
typedef math::XYZTLorentzVector LorentzVector;
/// Lorentz vector
typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;

//
// static data member definitions
//

//
// constructors and destructor
//
WJetFilter::WJetFilter(const edm::ParameterSet& iConfig) :
    isData_              ( iConfig.getParameter<bool  > ( "isData"              )  ) ,
    minPtMuon_           ( iConfig.getParameter<double> ( "minPtMuon"           )  ) ,
    minPtElectron_       ( iConfig.getParameter<double> ( "minPtElectron"       )  ) ,
    minPtMET_            ( iConfig.getParameter<double> ( "minPtMET"            )  ) ,
    minMt_               ( iConfig.getParameter<double> ( "minMt"               )  ) ,
    maxMt_               ( iConfig.getParameter<double> ( "maxMt"               )  ) ,
    minDeltaR_           ( iConfig.getParameter<double> ( "minDeltaR"           )  ) ,
    maxDeltaPhi_         ( iConfig.getParameter<double> ( "maxDeltaPhi"         )  ) ,
    minPtSelectedJet_    ( iConfig.getParameter<double> ( "minPtSelectedJet"    )  ) ,
    maxPtAdditionalJets_ ( iConfig.getParameter<double> ( "maxPtAdditionalJets" )  ) ,
    electronID_          ( iConfig.getParameter<string> ( "electronID"          )  ) ,
    output( { -1, -1, -1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<double>(), std::vector<double>(), std::vector<double>() } )
{
   //now do what ever initialization is needed
  LogTrace("WJetFilter") << "Constructing WJetFilter";

  alias_ = iConfig.getParameter<string>("@module_label");

  muonCollectionToken_ = consumes< pat::MuonCollection > (iConfig.getParameter<edm::InputTag>("srcMuons"));
  electronCollectionToken_ = consumes< pat::ElectronCollection > (iConfig.getParameter<edm::InputTag>("srcElectrons"));
  jetCollectionToken_ = consumes< pat::JetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));
  metCollectionToken_ = consumes< pat::METCollection > (iConfig.getParameter<edm::InputTag>("srcMET"));

  //Register products
  // produces< bool > ("zValidity")  . setBranchAlias( string("zValidity_").append(alias_) );
  // produces< PolarLorentzVector > ("zP4")  . setBranchAlias( string("zP4_").append(alias_) );
  //
  // produces< reco::PFJetCollection > (alias_)  . setBranchAlias( alias_ );
  produces< reco::PFJetCollection > ();
  // produces< vector<double> > ("deltaR")  . setBranchAlias( string("deltaR_").append(alias_) );
  // produces< vector<double> > ("deltaPhi")  . setBranchAlias( string("deltaPhi_").append(alias_) );
  // produces< bool > ("eventPassed")  . setBranchAlias( string("eventPassed_").append(alias_) );

  edm::Service<TFileService> fs;
  string name;
  name="eventCountPreFilter"  ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 2 , 0., 2.) );
  name="eventCountPostFilter" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 2 , 0., 2.) );

  name="pt_met"  ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 1000.) );

  name="mT_e"  ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 200.) );
  name="mT_mu" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 200.) );

  name="nGoodLepton" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nSelectedJet" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );

  name="pt_jetIndex"  ; histoMap2D_.emplace( name , fs->make<TH2D>(name.c_str() , name.c_str() , 100 , 0.  , 1000. , 10 , 0. , 10. )) ;

  name="pt_selectedJet" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 200.) );

  name="dPhi_jet_met"    ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 5.) ) ;
  name="dPhi_jet_lepton" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 5.) ) ;
  name="dPhi_jet_w"      ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 5.) ) ;

  for ( auto const & it : histoMap1D_ ) {
    it.second->Sumw2();
  }
  for ( auto const & it : histoMap2D_ ) {
    it.second->Sumw2();
  }

  outputTree = fs->make<TTree>("WJetFilterTree", "WJetFilterTree");
  outputTree->Branch("run"            , &output.run            ) ;
  outputTree->Branch("lumi"           , &output.lumi           ) ;
  outputTree->Branch("event"          , &output.event          ) ;
  outputTree->Branch("gen_weight"     , &output.gen_weight     ) ;
  outputTree->Branch("met_pt"         , &output.met_pt         ) ;
  outputTree->Branch("met_phi"        , &output.met_phi        ) ;
  outputTree->Branch("lepton_pt"      , &output.lepton_pt      ) ;
  outputTree->Branch("lepton_eta"     , &output.lepton_eta     ) ;
  outputTree->Branch("lepton_phi"     , &output.lepton_phi     ) ;
  outputTree->Branch("lepton_relIso"  , &output.lepton_relIso  ) ;
  outputTree->Branch("mT_e"           , &output.mT_e           ) ;
  outputTree->Branch("mT_u"           , &output.mT_u           ) ;
  outputTree->Branch("WpT_e"          , &output.WpT_e          ) ;  
  outputTree->Branch("WpT_u"          , &output.WpT_u          ) ;
  outputTree->Branch("jets_pt"        , &output.jets_pt        ) ;
  outputTree->Branch("jets_eta"       , &output.jets_eta       ) ;
  outputTree->Branch("jets_phi"       , &output.jets_phi       ) ;
}


WJetFilter::~WJetFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
WJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // eventPassed is set to false if an event fails selection,
  // but should still be taken to end of loop (e.g. for tree filling)
  // The alternative is to immediately return false.
  bool eventPassed = true;

  using namespace edm;
  output.run           = iEvent.id().run();
  output.event         = iEvent.id().event();
  output.lumi          = iEvent.id().luminosityBlock();
  output.gen_weight    = 1.0;
  output.met_pt        = -10;
  output.met_phi       = -10;
  output.lepton_pt     = -10;
  output.lepton_eta    = -10;
  output.lepton_phi    = -10;
  output.lepton_relIso = -10;
  output.mT_e          = -10;
  output.mT_u          = -10;
  output.WpT_e         = -10;
  output.WpT_u         = -10;
  output.jets_pt.clear();
  output.jets_eta.clear();
  output.jets_phi.clear();

  histoMap1D_["eventCountPreFilter"]->Fill(1.);

  edm::Handle< pat::MuonCollection > muonCollection;
  iEvent.getByToken(muonCollectionToken_, muonCollection);
  edm::Handle< pat::ElectronCollection > electronCollection;
  iEvent.getByToken(electronCollectionToken_, electronCollection);
  edm::Handle< pat::JetCollection > jetCollection;
  iEvent.getByToken(jetCollectionToken_, jetCollection);
  edm::Handle< pat::METCollection > metCollection;
  iEvent.getByToken(metCollectionToken_, metCollection);
  auto met = (metCollection->at(0));
  histoMap1D_["pt_met"]->Fill(met.pt());
  if ( met.pt() < minPtMET_ ) return false;
  output.met_pt = met.pt();
  output.met_phi = met.phi();

  ////////////////////////////////////////////////////////////
  // Calculate mT with electrons/muons
  ////////////////////////////////////////////////////////////

  // Construct zP4 from electrons
  pat::ElectronCollection goodElectrons;
  int nGoodLepton = 0;
  {
    pat::ElectronCollection& goodLeptons = goodElectrons;
    // Select good leptons
    auto leptons = electronCollection.product();
    // Lepton-specific criteria
    for ( auto it = leptons->begin(); it != leptons->end(); it++ ) {
      auto lepton = *it;
      if ( lepton.pt() < minPtElectron_ ) continue;
      if ( abs(lepton.eta()) > 2.5 ) continue;
      if ( lepton.electronID(electronID_) < 1 ) continue;
      double relIso = (lepton.trackIso() + lepton.caloIso()) / lepton.pt();
      output.lepton_relIso = relIso;
      nGoodLepton++;
      goodLeptons.push_back(lepton);
    }

    if ( goodLeptons.size() == 1 ) {
      float dPhi = TMath::Abs( ROOT::Math::VectorUtil::DeltaPhi( met.p4(), goodLeptons[0].p4() ) );
      double mT = TMath::Sqrt( 2. * goodLeptons[0].pt() * met.pt() * ( 1 - TMath::Cos(dPhi) ) );
      double WpT = TMath::Sqrt( TMath::Power(goodLeptons[0].pt(), 2) + TMath::Power(met.pt(), 2) + 2. * goodLeptons[0].pt() * met.pt() * TMath::Cos(dPhi) );
      LogTrace("WJetFilter") << "mT_e: " << mT;
      histoMap1D_["mT_e"]->Fill(mT);
      output.mT_e = mT;
      output.WpT_e = WpT;
      output.lepton_pt  = goodLeptons[0].pt();
      output.lepton_eta = goodLeptons[0].eta();
      output.lepton_phi = goodLeptons[0].phi();
    }
  }

  pat::MuonCollection goodMuons;
  {
    pat::MuonCollection& goodLeptons = goodMuons;
    // Select good leptons
    auto leptons = muonCollection.product();
    // Lepton-specific criteria
    for ( auto it = leptons->begin(); it != leptons->end(); it++ ) {
      auto lepton = *it;
      if ( lepton.pt() < minPtElectron_ ) continue;
      if ( abs(lepton.eta()) > 2.5 ) continue;
      if ( !lepton.muonID("AllGlobalMuons") ) continue;
      double relIso = (lepton.trackIso() + lepton.caloIso()) / lepton.pt();
      output.lepton_relIso = relIso;
      if ( relIso > 0.2 ) continue;
      nGoodLepton++;
      goodLeptons.push_back(lepton);
    }

    if ( goodLeptons.size() == 1 ) {
      float dPhi = TMath::Abs( ROOT::Math::VectorUtil::DeltaPhi( met.p4(), goodLeptons[0].p4() ) );
      double mT = TMath::Sqrt( 2. * goodLeptons[0].pt() * met.pt() * ( 1 - TMath::Cos(dPhi) ) );
      double WpT = TMath::Sqrt( TMath::Power(goodLeptons[0].pt(), 2) + TMath::Power(met.pt(), 2) + 2. * goodLeptons[0].pt() * met.pt() * TMath::Cos(dPhi) );
      LogTrace("WJetFilter") << "mT_mu " << mT;
      histoMap1D_["mT_mu"]->Fill(mT);
      output.mT_u = mT;
      output.WpT_u = WpT;
      output.lepton_pt  = goodLeptons[0].pt();
      output.lepton_eta = goodLeptons[0].eta();
      output.lepton_phi = goodLeptons[0].phi();
    }
  }

  // Get good lepton P4
  // LogTrace("WJetFilter") << "nGoodLepton " << nGoodLepton;
  histoMap1D_["nGoodLepton"]->Fill(nGoodLepton);
  if ( nGoodLepton != 1 ) return false;
  RecoLorentzVector goodLeptonP4; 
  if ( goodMuons.size()==1 ) goodLeptonP4 = goodMuons[0].p4();
  else                       goodLeptonP4 = goodElectrons[0].p4();

  // Require mT to pass selection cuts, but don't return until output has been filled
  assert ( goodMuons.size()+goodElectrons.size()==1 );
  double mT = -10;
  double WpT = -10;
  if ( goodMuons.size()==1 ) { mT = output.mT_u; WpT = output.WpT_u; }
  else                       { mT = output.mT_e; WpT = output.WpT_e; }
  if ( mT < minMt_ || mT > maxMt_ ) eventPassed = false;
  //if ( WpT < 35.0  || WpT > 90.0 ) eventPassed = false; 

  // Select jets that are more than minDeltaR_ away from the lepton,
  // and with pt > minPtSelectedJet_
  auto jets = jetCollection.product();
  reco::PFJetCollection selectedJet;
  // LogTrace("WJetFilter") << "Number of jets in event: " << jets->size();
  int nJetSelected = 0;
  int jetIndex = 0;
  for ( auto jet = jets->begin(); jet!= jets->end(); jet++ ) {

    if ( ROOT::Math::VectorUtil::DeltaR( goodLeptonP4, jet->p4() ) < minDeltaR_ ) continue;
    if (jet->pt() < minPtSelectedJet_) continue;
    const reco::Candidate* recoJet = jet->originalObject();
    if ( recoJet && jet->isPFJet() ) {
      if ( const reco::PFJet* pfJet = dynamic_cast<const reco::PFJet*>(recoJet) ) {
        selectedJet.push_back( *pfJet );
        nJetSelected++;
        histoMap1D_["pt_selectedJet"]->Fill( pfJet->pt() );
        histoMap2D_["pt_jetIndex"]->Fill( pfJet->pt(), jetIndex );
        // Store jet info in Tree for first four selected jets
        if ( nJetSelected <= 4 ) {
          LogTrace("WJetFilter") << "jet_pt: " << jet->pt();
          output.jets_pt.push_back(jet->pt());
          output.jets_eta.push_back(jet->eta());
          output.jets_phi.push_back(jet->phi());
        }
      }
    }

    jetIndex++;
  }
  
  LogTrace("WJetFilter") << "nJetSelected " << nJetSelected;
  histoMap1D_["nSelectedJet"]->Fill(selectedJet.size());

  outputTree->Fill();

  if ( selectedJet.size() == 0 ) return false;
  // if ( selectedJet.size() != 1 ) return false;
  auto jet = selectedJet[0];
  float dPhi_jet_met = ROOT::Math::VectorUtil::DeltaPhi( met.p4(), jet.p4() );
  histoMap1D_["dPhi_jet_met"]->Fill(dPhi_jet_met);
  auto wp4 = goodLeptonP4 + met.p4();
  float dPhi_jet_lepton = ROOT::Math::VectorUtil::DeltaPhi( goodLeptonP4, jet.p4() );
  histoMap1D_["dPhi_jet_lepton"]->Fill(dPhi_jet_lepton);
  float dPhi_jet_w = ROOT::Math::VectorUtil::DeltaPhi( wp4, jet.p4() );
  histoMap1D_["dPhi_jet_w"]->Fill(dPhi_jet_w);

  histoMap1D_["eventCountPostFilter"]->Fill(1.);

    // float dPhi = ROOT::Math::VectorUtil::DeltaPhi( met.p4(), jet.p4() );
    // const reco::Candidate* recoJet = jet->originalObject();
    // if (!recoJet) LogTrace("WJetFilter") << "originalObject empty!";
    // else {
    //   if ( const reco::PFJet* pfJet = dynamic_cast<const reco::PFJet*>(recoJet) ) {
    //     LogTrace("WJetFilter") << "originalObject successfully casted to PFJet!";
    //   }
    //   else {
    //     LogTrace("WJetFilter") << "originalObject could not be casted to PFJet!";
    //   }
    // }
  // }


  // LogTrace("WJetFilter") << "Printing electron pts.";
  // for ( auto it = electrons->begin(); it != electrons->end(); it++ ) {
  //   auto electron = *it;
  //   LogTrace("WJetFilter") << electron.pt();
  // }


  // std::auto_ptr< bool > _zValidity( new bool (zValidity) );
  // iEvent.put(_zValidity, "zValidity");
  // std::auto_ptr< PolarLorentzVector > _zP4( new PolarLorentzVector (zP4) );
  // iEvent.put(_zP4, "zP4");

  // LogTrace("WJetFilter") << "eventPassed: " << eventPassed;

  std::auto_ptr< reco::PFJetCollection > _selectedJet( new reco::PFJetCollection (selectedJet) );
  iEvent.put(_selectedJet);
  // std::auto_ptr< vector<double> > _deltaRs( new vector<double> (deltaRs) );
  // iEvent.put(_deltaRs, "deltaR");
  // std::auto_ptr< vector<double> > _deltaPhis( new vector<double> (deltaPhis) );
  // iEvent.put(_deltaPhis, "deltaPhi");
  // std::auto_ptr< bool > _eventPassed( new bool (eventPassed) );
  // iEvent.put(_eventPassed, "eventPassed");

  return eventPassed;
}

// ------------ method called once each job just before starting event loop  ------------
void 
WJetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
WJetFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
WJetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
WJetFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
WJetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
WJetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
WJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(WJetFilter);
