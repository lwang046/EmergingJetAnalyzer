// -*- C++ -*-
//
// Package:    EmergingAnalysis/GJetFilter
// Class:      GJetFilter
// 
/**\class GJetFilter GJetFilter.cc EmergingAnalysis/GJetFilter/plugins/GJetFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Young Ho Shin
//         Created:  Fri, 03 Feb 2017 14:57:24 GMT
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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// Namespace shorthands
using std::string;
using std::vector;

//
// class declaration
//

class GJetFilter : public edm::EDFilter {
  public:
    explicit GJetFilter(const edm::ParameterSet&);
    ~GJetFilter();

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
    edm::EDGetTokenT< pat::PhotonCollection > photonCollectionToken_;    
    edm::EDGetTokenT< pat::JetCollection > jetCollectionToken_;
    double minPtPhoton_;
    double minDeltaR_;
    double maxDeltaPhi_;
    double minPtSelectedJet_;
    double maxPtAdditionalJets_;
    string photonID_;

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
        double photon_pt;
        double photon_eta;
        double photon_phi;
        double photon_relIso;
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
GJetFilter::GJetFilter(const edm::ParameterSet& iConfig) :
    isData_              ( iConfig.getParameter<bool  > ( "isData"              )  ) ,
    minPtPhoton_         ( iConfig.getParameter<double> ( "minPtPhoton"         )  ) ,
    minDeltaR_           ( iConfig.getParameter<double> ( "minDeltaR"           )  ) ,
    maxDeltaPhi_         ( iConfig.getParameter<double> ( "maxDeltaPhi"         )  ) ,
    minPtSelectedJet_    ( iConfig.getParameter<double> ( "minPtSelectedJet"    )  ) ,
    maxPtAdditionalJets_ ( iConfig.getParameter<double> ( "maxPtAdditionalJets" )  ) ,
    photonID_            ( iConfig.getParameter<string> ( "photonID"            )  ) ,
    output( { -1, -1, -1, 0.0, 0.0, 0.0, 0.0, 0.0, std::vector<double>(), std::vector<double>(), std::vector<double>() } )
{
   //now do what ever initialization is needed
  LogTrace("GJetFilter") << "Constructing GJetFilter";

  alias_ = iConfig.getParameter<string>("@module_label");

  photonCollectionToken_ = consumes< pat::PhotonCollection > (iConfig.getParameter<edm::InputTag>("srcPhotons"));
  jetCollectionToken_ = consumes< pat::JetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));

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

  name="nPhoton" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nGoodPhoton1" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nGoodPhoton2" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nGoodPhoton3" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nGoodPhoton4" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );
  name="nSelectedJet" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 20 , 0., 20.) );

  name="pt_jetIndex"  ; histoMap2D_.emplace( name , fs->make<TH2D>(name.c_str() , name.c_str() , 100 , 0.  , 1000. , 10 , 0. , 10. )) ;

  name="pt_selectedJet" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 200.) );

  name="dPhi_jet_photon" ; histoMap1D_.emplace( name , fs->make<TH1D>(name.c_str() , name.c_str() , 100 , 0., 5.) ) ;

  for ( auto const & it : histoMap1D_ ) {
    it.second->Sumw2();
  }
  for ( auto const & it : histoMap2D_ ) {
    it.second->Sumw2();
  }

  outputTree = fs->make<TTree>("GJetFilterTree", "GJetFilterTree");
  outputTree->Branch("run"            , &output.run            ) ;
  outputTree->Branch("lumi"           , &output.lumi           ) ;
  outputTree->Branch("event"          , &output.event          ) ;
  outputTree->Branch("gen_weight"     , &output.gen_weight     ) ;
  outputTree->Branch("photon_pt"      , &output.photon_pt      ) ;
  outputTree->Branch("photon_eta"     , &output.photon_eta     ) ;
  outputTree->Branch("photon_phi"     , &output.photon_phi     ) ;
  outputTree->Branch("photon_relIso"  , &output.photon_relIso  ) ;
  outputTree->Branch("jets_pt"        , &output.jets_pt        ) ;
  outputTree->Branch("jets_eta"       , &output.jets_eta       ) ;
  outputTree->Branch("jets_phi"       , &output.jets_phi       ) ;
}


GJetFilter::~GJetFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
GJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
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
  output.photon_pt     = -10;
  output.photon_eta    = -10;
  output.photon_phi    = -10;
  output.photon_relIso = -10;
  output.jets_pt.clear();
  output.jets_eta.clear();
  output.jets_phi.clear();

  histoMap1D_["eventCountPreFilter"]->Fill(1.);

  edm::Handle< pat::JetCollection > jetCollection;
  iEvent.getByToken(jetCollectionToken_, jetCollection);
  edm::Handle< pat::PhotonCollection > photonCollection;  
  iEvent.getByToken(photonCollectionToken_, photonCollection);

  //
  //Photon Selection
  //
  pat::PhotonCollection goodPhotons;
  int nPhoton = 0;
  int nGoodPhoton1 = 0;
  int nGoodPhoton2 = 0;
  int nGoodPhoton3 = 0;
  int nGoodPhoton4 = 0;
  {
    // Select good photons
    auto photons = photonCollection.product();
    // Photon-specific criteria
    for ( auto it = photons->begin(); it != photons->end(); it++ ) {
      auto photon = *it;
      nPhoton++;
      if ( photon.pt() < minPtPhoton_ ) continue;//photon pT selection
      //if( photon.pt() < 35.0 || photon.pt() > 90.0 ) continue;
      nGoodPhoton1++;
      if ( fabs(photon.eta()) > 1.4442 ) continue;
      nGoodPhoton2++;
      if ( photon.hasPixelSeed() ) continue;
      nGoodPhoton3++;
      if ( !photon.photonID(photonID_) ) continue;
      nGoodPhoton4++;
      //if( photon.hadTowOverEm()>0.05 || photon.full5x5_sigmaIetaIeta()>0.0102 || photon.hasPixelSeed() ) continue;
      //if( photon.hadTowOverEm()>0.05 || photon.full5x5_sigmaIetaIeta()>0.0102 ) continue;
      //if( photon.hadTowOverEm()>0.05 ) continue;
      double relIso = (photon.trackIso() + photon.caloIso()) / photon.pt();
      output.photon_relIso = relIso;
      goodPhotons.push_back(photon);
    }

    if ( goodPhotons.size() == 1 ) {
      output.photon_pt  = goodPhotons[0].pt();
      output.photon_eta = goodPhotons[0].eta();
      output.photon_phi = goodPhotons[0].phi();
    }
  }


  // Get good photon P4
  // LogTrace("GJetFilter") << "nGoodPhoton " << nGoodPhoton;
  histoMap1D_["nPhoton"]->Fill(nPhoton);
  histoMap1D_["nGoodPhoton1"]->Fill(nGoodPhoton1);
  histoMap1D_["nGoodPhoton2"]->Fill(nGoodPhoton2);
  histoMap1D_["nGoodPhoton3"]->Fill(nGoodPhoton3);
  histoMap1D_["nGoodPhoton4"]->Fill(nGoodPhoton4);

  if ( nGoodPhoton4 != 1 ) return false;

  RecoLorentzVector goodPhotonP4; 
  goodPhotonP4 = goodPhotons[0].p4();


  // Select jets that are more than minDeltaR_ away from the photon,
  // and with pt > minPtSelectedJet_
  auto jets = jetCollection.product();
  reco::PFJetCollection selectedJet;
  // LogTrace("GJetFilter") << "Number of jets in event: " << jets->size();
  int nJetSelected = 0;
  int jetIndex = 0;
  for ( auto jet = jets->begin(); jet!= jets->end(); jet++ ) {

    if ( ROOT::Math::VectorUtil::DeltaR( goodPhotonP4, jet->p4() ) < minDeltaR_ ) continue;
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
          LogTrace("GJetFilter") << "jet_pt: " << jet->pt();
          output.jets_pt.push_back(jet->pt());
          output.jets_eta.push_back(jet->eta());
          output.jets_phi.push_back(jet->phi());
        }
      }
    }

    jetIndex++;
  }
  
  LogTrace("GJetFilter") << "nJetSelected " << nJetSelected;
  histoMap1D_["nSelectedJet"]->Fill(selectedJet.size());

  outputTree->Fill();

  if ( selectedJet.size() == 0 ) return false;
  // if ( selectedJet.size() != 1 ) return false;
  auto jet = selectedJet[0];
  float dPhi_jet_photon = ROOT::Math::VectorUtil::DeltaPhi( goodPhotonP4, jet.p4() );
  histoMap1D_["dPhi_jet_photon"]->Fill(dPhi_jet_photon);

  histoMap1D_["eventCountPostFilter"]->Fill(1.);


  // LogTrace("GJetFilter") << "Printing photon pts.";
  // for ( auto it = photons->begin(); it != photons->end(); it++ ) {
  //   auto photon = *it;
  //   LogTrace("GJetFilter") << photon.pt();
  // }


  // std::auto_ptr< bool > _zValidity( new bool (zValidity) );
  // iEvent.put(_zValidity, "zValidity");
  // std::auto_ptr< PolarLorentzVector > _zP4( new PolarLorentzVector (zP4) );
  // iEvent.put(_zP4, "zP4");

  // LogTrace("GJetFilter") << "eventPassed: " << eventPassed;

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
GJetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GJetFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
GJetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
GJetFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
GJetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
GJetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(GJetFilter);
