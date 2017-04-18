// -*- C++ -*-
//
// Package:    EmergingAnalysis/GenJetFilter
// Class:      GenJetFilter
// 
/**\class GenJetFilter GenJetFilter.cc EmergingAnalysis/GenJetFilter/plugins/GenJetFilter.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Young Ho Shin
//         Created:  Wed, 19 Aug 2015 22:53:22 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Data formats
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

//
// class declaration
//

class GenJetFilter : public edm::EDFilter {
  public:
    explicit GenJetFilter(const edm::ParameterSet&);
    ~GenJetFilter();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    edm::EDGetTokenT< reco::GenJetCollection > jetCollectionToken_;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//
const double minPt_ = 0.0;

//
// static data member definitions
//

//
// constructors and destructor
//
GenJetFilter::GenJetFilter(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  jetCollectionToken_ = consumes< reco::GenJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));
  produces<double> ("genHt").setBranchAlias("genHt");
}


GenJetFilter::~GenJetFilter()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
  bool
GenJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  edm::Handle< reco::GenJetCollection > jetCollection;
  iEvent.getByToken(jetCollectionToken_, jetCollection);
  auto jets = *(jetCollection.product());

  double genHt = 0;
  for ( auto jet : jets ) {
  // for ( auto jet = jets->begin(); jet != jets->end(); jet++ ) {
    if ( jet.pt() > minPt_ ) {
      genHt += jet.pt();
    }
  }
  std::auto_ptr<double> _genHt( new double (genHt) );
  iEvent.put(_genHt, "genHt");

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
  void 
GenJetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenJetFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   GenJetFilter::beginRun(edm::Run const&, edm::EventSetup const&)
   { 
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   GenJetFilter::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   GenJetFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   GenJetFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(GenJetFilter);
