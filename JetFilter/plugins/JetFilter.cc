// -*- C++ -*-
//
// Package:    EmergingAnalysis/JetFilter
// Class:      JetFilter
// 
/**\class JetFilter JetFilter.cc EmergingAnalysis/JetFilter/plugins/JetFilter.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Young Ho Shin
//         Created:  Fri, 17 Jul 2015 13:01:06 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

// For creating/writing histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"

// Data formats
#include "DataFormats/JetReco/interface/PFJetCollection.h"

// Utils
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

// Namespace shorthands
using std::string;
using std::vector;

//
// class declaration
//

class JetFilter : public edm::EDFilter {
  public:
    explicit JetFilter(const edm::ParameterSet&);
    ~JetFilter();

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
    ////////////////////////////////////////
    // inputs
    ////////////////////////////////////////
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;
    std::vector<double> minPts_;  // minPts_[i] corresponds to the i-th minimum pt-cut
    std::vector<double> maxEtas_; // maxEtas_[i] corresponds to the i-th max eta-cut
    std::vector<StringCutObjectSelector<reco::PFJet> > stringCutSelectors_; // String cuts for the i-th jet
    int nCuts_;

    // Generic cuts for the jets that don't pass the previous criteria
    StringCutObjectSelector<reco::PFJet> additionalCutSelector_;

    ////////////////////////////////////////
    // outputs
    ////////////////////////////////////////
    std::auto_ptr< reco::PFJetCollection > selectedJets_;
    std::vector<int> selectedJetsIndex_;

    std::unordered_map<string, TH1*> histoMap1D_;
    std::unordered_map<string, TH2*> histoMap2D_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JetFilter::JetFilter(const edm::ParameterSet& iConfig):
  additionalCutSelector_( StringCutObjectSelector<reco::PFJet>(iConfig.getParameter<std::string>("additionalCut")) )
{
  LogDebug("JetFilter") << "Construcing JetFilter";
  //now do what ever initialization is needed
  jetCollectionToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));
  vector<edm::ParameterSet> jetCuts(iConfig.getParameter<std::vector<edm::ParameterSet> >("jetCuts"));
  for (auto jetCut = jetCuts.begin(); jetCut != jetCuts.end(); jetCut++) {
    double minPt  = jetCut->getParameter<double>("minPt");
    double maxEta = jetCut->getParameter<double>("maxEta");
    string stringCut = jetCut->getParameter<std::string>("stringCut");
    StringCutObjectSelector<reco::PFJet> stringCutSelector(stringCut);
    LogDebug("JetFilter") << "Printing pt-cut for jets" << "minPt: " << minPt;

    minPts_.push_back(minPt);
    maxEtas_.push_back(maxEta);
    stringCutSelectors_.push_back(stringCutSelector);
  }
  nCuts_ = jetCuts.size();

  // additionalCutString = iConfig.getParameter<std::string>("additionalCut");
  // additionalCutSelector_ = StringCutObjectSelector<reco::PFJet>(additionalCutString);

  edm::Service<TFileService> fs;
  string name;
  name="jetPt_jetIndex"  ; histoMap2D_.emplace( name , fs->make<TH2D>(name.c_str() , name.c_str() , 100 , 0.  , 500. , 10 , 0. , 10. )) ;
  name="jetEta_jetIndex" ; histoMap2D_.emplace( name , fs->make<TH2D>(name.c_str() , name.c_str() , 100 , -5. , 5.   , 10 , 0. , 10. ))  ;


  produces< reco::PFJetCollection > ("selectedJets"). setBranchAlias( "selectedJets" );
}


JetFilter::~JetFilter()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
  bool
JetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  bool eventPassed = false;

  edm::Handle< reco::PFJetCollection > jetCollection;
  iEvent.getByToken(jetCollectionToken_, jetCollection);

  std::auto_ptr< reco::PFJetCollection > selectedJets_( new reco::PFJetCollection() );
  selectedJets_->reserve(jetCollection->size());

  LogDebug("JetFilter") << "Iterating over jets";
  int nJetsPassing = 0;
  int jetIndex = 0;
  for (auto it = jetCollection->begin(); it != jetCollection->end(); it++) {
    auto jet = *it;
    if (nJetsPassing < nCuts_) {
      if (fabs(jet.eta()) < maxEtas_[nJetsPassing] && jet.pt() > minPts_[nJetsPassing]) {
        bool passStringCut = stringCutSelectors_[nJetsPassing](jet);
        if (passStringCut) {
          selectedJets_->push_back(jet);
          selectedJetsIndex_.push_back(jetIndex);
          nJetsPassing++;
        }
      }
    } else if (nJetsPassing > 3) {
      eventPassed = true;
      break;
    }
    jetIndex++;
  }

  if (eventPassed) {
    int jetIndex = 0;
    for (auto it = jetCollection->begin(); it != jetCollection->end(); it++) {
      auto jet = *it;
      auto find = std::find(selectedJetsIndex_.begin(), selectedJetsIndex_.end(), jetIndex);
      // If jet has NOT been selected already
      if (find == selectedJetsIndex_.end()) {
        bool jetPassed = additionalCutSelector_(jet);
        if (jetPassed) {
          selectedJets_->push_back(jet);
          nJetsPassing++;
        }
      }
      jetIndex++;
    }
  }

  iEvent.put(selectedJets_, "selectedJets");

  // std::auto_ptr< double > _dummy( new double (0.0) );
  // iEvent.put(_dummy, "dummy");

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
  return eventPassed;
}

// ------------ method called once each job just before starting event loop  ------------
  void 
JetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   JetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
   { 
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   JetFilter::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   JetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   JetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(JetFilter);
