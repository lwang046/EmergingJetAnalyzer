// -*- C++ -*-
//
// Package:    EmergingJetAnalysis/ZJetFilter
// Class:      ZJetFilter
// 
/**\class ZJetFilter ZJetFilter.cc EmergingJetAnalysis/ZJetFilter/plugins/ZJetFilter.cc

Description: Selects Z+Jet events where there is exactly one hard jet back to back with the Z boson.

Implementation:
// Requires Z boson 4-vector to be present in the event. (reco::Candidate::LorentzVector)
Saves selected hard jet.
*/
//
// Original Author:  Young Ho Shin
//         Created:  Mon, 08 Jun 2015 15:35:15 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TVector3.h>
#include <Math/VectorUtil.h>

// For creating/writing histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

// Data formats
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// #include "DataFormats/JetReco/interface/PFJetCollection.h"

// Namespace shorthands
using std::string;
using std::vector;

//
// class declaration
//

class ZJetFilter : public edm::EDFilter {
  public:
    explicit ZJetFilter(const edm::ParameterSet&);
    ~ZJetFilter();

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
    // Retrieve once 
    string alias_; // Alias suffix for all products
    // Retrieve once per event
    edm::EDGetTokenT< pat::MuonCollection > muonCollectionToken_;
    edm::EDGetTokenT< pat::ElectronCollection > electronCollectionToken_;
    edm::EDGetTokenT< pat::JetCollection > jetCollectionToken_;
    double minPtMuon_;
    double minPtElectron_;
    double minZMass_;
    double maxZMass_;
    double maxDeltaPhi_;
    double minPtSelectedJet_;
    double maxPtAdditionalJets_;

    // Outputs
    TTree* outputTree;
    struct outputClass {
      public:
        double m_ee;
        double m_uu;
        vector<double> jets_pt;
        vector<double> jets_eta;
        vector<double> jets_phi;
        vector<double> jets_dPhi_Z;
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
ZJetFilter::ZJetFilter(const edm::ParameterSet& iConfig) :
    minPtMuon_           (  iConfig.getParameter<double>("minPtMuon") ),
    minPtElectron_       (  iConfig.getParameter<double>("minPtElectron") ),
    minZMass_            (  iConfig.getParameter<double>("minZMass") ),
    maxZMass_            (  iConfig.getParameter<double>("maxZMass") ),
    maxDeltaPhi_         (  iConfig.getParameter<double>("maxDeltaPhi") ),
    minPtSelectedJet_    (  iConfig.getParameter<double>("minPtSelectedJet") ),
    maxPtAdditionalJets_ (  iConfig.getParameter<double>("maxPtAdditionalJets") ),
    output( {0.0, 0.0, std::vector<double>(), std::vector<double>(), std::vector<double>(), std::vector<double>() } )
{
  //now do what ever initialization is needed

  alias_ = iConfig.getParameter<string>("@module_label");

  muonCollectionToken_ = consumes< pat::MuonCollection > (iConfig.getParameter<edm::InputTag>("srcMuons"));
  electronCollectionToken_ = consumes< pat::ElectronCollection > (iConfig.getParameter<edm::InputTag>("srcElectrons"));
  jetCollectionToken_ = consumes< pat::JetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));

  //Register products
  // produces< bool > ("zValidity")  . setBranchAlias( string("zValidity_").append(alias_) );
  produces< PolarLorentzVector > ("zP4")  . setBranchAlias( string("zP4_").append(alias_) );
  //
  // produces< pat::JetCollection > ("jetSelected")  . setBranchAlias( string("jetSelected_").append(alias_) );
  // produces< vector<double> > ("deltaR")  . setBranchAlias( string("deltaR_").append(alias_) );
  // produces< vector<double> > ("deltaPhi")  . setBranchAlias( string("deltaPhi_").append(alias_) );
  // produces< bool > ("eventPassed")  . setBranchAlias( string("eventPassed_").append(alias_) );

  edm::Service<TFileService> fs;
  outputTree = fs->make<TTree>("ZJetFilterTree", "ZJetFilterTree");
  outputTree->Branch("m_ee"        , &output.m_ee        ) ;
  outputTree->Branch("m_uu"        , &output.m_uu        ) ;
  outputTree->Branch("jets_pt"     , &output.jets_pt     ) ;
  outputTree->Branch("jets_eta"    , &output.jets_eta    ) ;
  outputTree->Branch("jets_phi"    , &output.jets_phi    ) ;
  outputTree->Branch("jets_dPhi_Z" , &output.jets_dPhi_Z ) ;
}


ZJetFilter::~ZJetFilter()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
  bool
ZJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle< pat::MuonCollection > muonCollection;
  iEvent.getByToken(muonCollectionToken_, muonCollection);
  edm::Handle< pat::ElectronCollection > electronCollection;
  iEvent.getByToken(electronCollectionToken_, electronCollection);
  edm::Handle< pat::JetCollection > jetCollection;
  iEvent.getByToken(jetCollectionToken_, jetCollection);

  ////////////////////////////////////////////////////////////
  // Calculate zP4 with electrons/muons
  ////////////////////////////////////////////////////////////
  bool zValidity_ee = false;
  bool zValidity_uu = false;
  PolarLorentzVector zP4;

  // Construct zP4 from electrons
  {
    // Select good leptons
    bool& zValidity = zValidity_ee;
    auto& m_ll = output.m_ee;
    auto leptons = electronCollection.product();
    int nGoodLepton = 0;
    pat::ElectronCollection goodLeptons;
    // Lepton-specific criteria
    for ( auto it = leptons->begin(); it != leptons->end(); it++ ) {
      auto lepton = *it;
      if ( lepton.pt() < minPtElectron_ ) continue;
      if ( abs(lepton.eta()) > 2.5 ) continue;
      if ( lepton.electronID("cutBasedElectronID-CSA14-PU20bx25-V0-standalone-medium") < 1 ) continue;
      nGoodLepton++;
      goodLeptons.push_back(lepton);
    }

    if ( goodLeptons.size() >= 2 ) {
      // Assuming collection is sorted by pt
      RecoLorentzVector zP4_;
      zP4_ = goodLeptons[0].p4() + goodLeptons[1].p4();
      LogTrace("ZJetFilter") << "Printing Z mass: " << zP4_.mass();
      m_ll = zP4_.mass();
      if ( minZMass_ < zP4_.mass() && zP4_.mass() < maxZMass_ ) {
        zP4 = zP4_;
        zValidity = true;
      }
    }
  }

  {
    // Select good leptons
    bool& zValidity = zValidity_uu;
    auto& m_ll = output.m_uu;
    auto leptons = muonCollection.product();
    int nGoodLepton = 0;
    pat::MuonCollection goodLeptons;
    // Lepton-specific criteria
    for ( auto it = leptons->begin(); it != leptons->end(); it++ ) {
      auto lepton = *it;
      if ( lepton.pt() < minPtElectron_ ) continue;
      if ( abs(lepton.eta()) > 2.5 ) continue;
      if ( !lepton.muonID("AllGlobalMuons") ) continue;
      nGoodLepton++;
      goodLeptons.push_back(lepton);
    }

    if ( goodLeptons.size() >= 2 ) {
      // Assuming collection is sorted by pt
      RecoLorentzVector zP4_;
      zP4_ = goodLeptons[0].p4() + goodLeptons[1].p4();
      LogTrace("ZJetFilter") << "Printing Z mass: " << zP4_.mass();
      m_ll = zP4_.mass();
      if ( minZMass_ < zP4_.mass() && zP4_.mass() < maxZMass_ ) {
        zP4 = zP4_;
        zValidity = true;
      }
    }
  }

  // LogTrace("ZJetFilter") << "Printing electron pts.";
  // for ( auto it = electrons->begin(); it != electrons->end(); it++ ) {
  //   auto electron = *it;
  //   LogTrace("ZJetFilter") << electron.pt();
  // }


  // std::auto_ptr< bool > _zValidity( new bool (zValidity) );
  // iEvent.put(_zValidity, "zValidity");
  // std::auto_ptr< PolarLorentzVector > _zP4( new PolarLorentzVector (zP4) );
  // iEvent.put(_zP4, "zP4");

  ////////////////////////////////////////////////////////////
  // Check if there is a jet opposite to Z
  // Veto presence of additional hard jets
  ////////////////////////////////////////////////////////////
  bool eventPassed = false;
  pat::JetCollection jetToSave;
  vector<double> deltaRs;
  vector<double> deltaPhis;
  if (zValidity_ee | zValidity_uu) {
    auto jets = jetCollection.product();
    pat::JetCollection::const_iterator jetSelected = jets->end();
    // Loop over jets to find first jet that is within dR cut of -zP4
    int iJet = 0;
    for ( auto jet = jets->begin(); jet!= jets->end(); jet++ ) {
      float dR = ROOT::Math::VectorUtil::DeltaR( jet->p4(), - zP4 );
      float dPhi = ROOT::Math::VectorUtil::DeltaPhi( jet->p4(), -zP4 );
      // std::cout << "dR: " << dR << " \n";
      // std::cout << "jet->pt(): " << jet->pt() << " \n";
      // std::cout << iJet << "\t" << dR << " \t" << jet->pt() << " \n";
      if ( dPhi < maxDeltaPhi_ && jet->pt() > minPtSelectedJet_ ) {
        eventPassed = true;
        jetSelected = jet;
        jetToSave.push_back(*jet);
        deltaRs.push_back(dR);
        deltaPhis.push_back(dPhi);
        break;
      }
      iJet++;
    }
    // Veto presence of additional hard jets
    for ( auto jet = jets->begin(); jet!= jets->end(); jet++ ) {
      if ( jet == jetSelected ) {
        continue;
      }
      else if ( jet->pt() > maxPtAdditionalJets_ ) {
        eventPassed = false;
        break;
      }
    }
  }
  LogTrace("ZJetFilter") << "eventPassed: " << eventPassed;

  // std::auto_ptr< reco::PFJetCollection > _jetToSave( new reco::PFJetCollection (jetToSave) );
  // iEvent.put(_jetToSave, "jetSelected");
  // std::auto_ptr< vector<double> > _deltaRs( new vector<double> (deltaRs) );
  // iEvent.put(_deltaRs, "deltaR");
  // std::auto_ptr< vector<double> > _deltaPhis( new vector<double> (deltaPhis) );
  // iEvent.put(_deltaPhis, "deltaPhi");
  // std::auto_ptr< bool > _eventPassed( new bool (eventPassed) );
  // iEvent.put(_eventPassed, "eventPassed");

  outputTree->Fill();
  return eventPassed;
}

// ------------ method called once each job just before starting event loop  ------------
  void 
ZJetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZJetFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   ZJetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
   { 
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   ZJetFilter::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   ZJetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   ZJetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(ZJetFilter);
