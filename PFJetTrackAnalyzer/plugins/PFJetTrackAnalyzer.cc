// -*- C++ -*-
//
// Package:    PFJetTrackAnalysis/PFJetTrackAnalyzer
// Class:      PFJetTrackAnalyzer
//
/**\class PFJetTrackAnalyzer PFJetTrackAnalyzer.cc PFJetTrackAnalysis/PFJetTrackAnalyzer/plugins/PFJetTrackAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Zishuo Yang
//         Created:  Fri, 22 Apr 2016 21:21:17 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"

//
// class declaration
//

class PFJetTrackAnalyzer : public edm::EDProducer {
    public:
        explicit PFJetTrackAnalyzer(const edm::ParameterSet&);
        ~PFJetTrackAnalyzer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() override;
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
        //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
        //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
        //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

        // ----------member data ---------------------------

        // inputs
        edm::EDGetTokenT<reco::PFJetCollection> jetToken_;

        // outputs
        TFile* ofile_;
        TTree* tree_;

        std::vector<int> PFJetIndex_;
        std::vector<float> PFJetPt_;
        std::vector<float> PFJetEta_;
        std::vector<float> PFJetPhi_;

        std::auto_ptr< reco::TrackCollection > associatedTracks_; // reco tracks associated with the jet
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
PFJetTrackAnalyzer::PFJetTrackAnalyzer(const edm::ParameterSet& iConfig)
{
    // ofile_ = new TFile("file.root","RECREATE","PFJetTrackAnalyzer output file");
    // tree_ = new TTree("PFJetTrackTree", "PFJetTrackAnalyzer output tree");
    // tree_->Branch("index"        , &PFJetIndex_                   ) ;
    // tree_->Branch("pt"           , &PFJetPt_                      ) ;
    // tree_->Branch("eta"          , &PFJetEta_                     ) ;
    // tree_->Branch("phi"          , &PFJetPhi_                     ) ;

    jetToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));

    produces< reco::TrackCollection> ("associatedTracks") . setBranchAlias( "associatedTracks" );
}


PFJetTrackAnalyzer::~PFJetTrackAnalyzer()
{
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
    // ofile_->Write();
    // delete ofile_;

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PFJetTrackAnalyzer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    edm::Handle<reco::PFJetCollection> inputJets;
    iEvent.getByToken(jetToken_, inputJets);
    PFJetIndex_                   . clear();
    PFJetPt_                      . clear();
    PFJetEta_                     . clear();
    PFJetPhi_                     . clear();
    std::auto_ptr< reco::TrackCollection > associatedTracks_( new reco::TrackCollection() );
    //associatedTracks_->reserve(1000);

    int jetIndex = 0;
    for (auto it = inputJets->begin(); it != inputJets->end(); it++) {
        auto& PFJet = *it;
        jetIndex++;
        PFJetIndex_          . push_back( jetIndex      );
        PFJetPt_             . push_back( PFJet.pt()    );
        PFJetEta_            . push_back( PFJet.eta()   );
        PFJetPhi_            . push_back( PFJet.phi()   );

        reco::TrackRefVector assoTracks = it->getTrackRefs();
        for (unsigned int i=0; i!=assoTracks.size(); ++i) {
            edm::RefToBase<reco::Track> track(assoTracks[i]);
            associatedTracks_ -> push_back( *assoTracks[i] );
        }


    }
    std::cout<<"Check the first track in assoTrackCollection by its pt: "<< (*associatedTracks_)[0].pt()<<std::endl;
    // tree_->Fill();
    iEvent.put(associatedTracks_, "associatedTracks");
}

// ------------ method called once each job just before starting event loop  ------------
void
PFJetTrackAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
PFJetTrackAnalyzer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   PFJetTrackAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   PFJetTrackAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   PFJetTrackAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   PFJetTrackAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PFJetTrackAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PFJetTrackAnalyzer);
