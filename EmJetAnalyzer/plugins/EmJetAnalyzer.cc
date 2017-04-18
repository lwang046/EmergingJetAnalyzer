// -*- C++ -*-
//
// Package:    EmergingJetAnalysis/EmJetAnalyzer
// Class:      EmJetAnalyzer
//
/**\class EmJetAnalyzer EmJetAnalyzer.cc EmergingJetAnalysis/EmJetAnalyzer/plugins/EmJetAnalyzer.cc

 Description: Analyzer for Emerging Jet analysis, supercedes EmergingJetAnalyzer

 Implementation:
     Re-write of EmergingJetAnalyzer to be more modular.
*/
//
// Original Author:  Young Ho Shin
//         Created:  Tue, 05 Apr 2016 19:37:25 GMT
//
//

// Useful keywords
// :MCONLY: Code that applies only when processing MC events
// :CUT: Code that applies cuts to objects/events
// :EVENTLEVEL: Code that calculates event-level quantities
// :JETLEVEL: Code that calculates jet-level quantities
// :JETTRACKLEVEL: Code that calculates jet-track-level quantities
// :JETSOURCE: Code that assigns "source" variable for a jet
// :TRACKSOURCE: Code that assigns "source" variable for a track
// :VERTEXSOURCE:


// system include files
#include <memory>
#include <stdlib.h> // For rand()

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/GeometrySurface/interface/Line.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/JetReco/interface/GenJet.h"

// Hit pattern
#include "PhysicsTools/RecoUtils/interface/CheckHitPattern.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

// Jet Tracks Association
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TParameter.h"

#include "EmergingJetAnalysis/EmJetAnalyzer/interface/OutputTree.h"
#include "EmergingJetAnalysis/EmJetAnalyzer/interface/EmJetEvent.h"
#include "EmergingJetAnalysis/GenParticleAnalyzer/plugins/GenParticleAnalyzer.cc"

#ifndef OUTPUT
#define OUTPUT(x) std::cout<<#x << ": " << x << std::endl
#endif

//
// constants, enums and typedefs
//
// bool scanMode_ = true;
// bool scanRandomJet_ = true;
bool jetdump_ = false;

// typedef std::vector<TransientVertex> TransientVertexCollection;

//
// class declaration
//

using namespace emjet;

class GenParticleAnalyzer;

class EmJetAnalyzer : public edm::EDFilter {
  public:
    explicit EmJetAnalyzer(const edm::ParameterSet&);
    ~EmJetAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // n-tuple filling
    void prepareJet(const reco::PFJet& ijet, Jet& ojet, int source);
    void prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source);
    void prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source);
    void prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup);
    void fillJet(const reco::PFJet& ijet, Jet& ojet);
    void fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack);
    void fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex);
    bool selectTrack(const reco::TransientTrack& itrack, const Track& otrack);
    bool selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetTrackDeltaR(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack);
    bool selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex);
    void fillGenParticles () ;

    // EDM output
    void findDarkPionVertices () ;

    // Computation functions
    double compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const;
    double compute_alphaMax(reco::TrackRefVector& trackRefs) const;
    int    compute_nDarkPions(const reco::PFJet& ijet) const;
    int    compute_nDarkGluons(const reco::PFJet& ijet) const;
    double compute_pt2Sum (const TransientVertex& ivertex) const;

    // Utility functions
    reco::TrackRefVector MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2);


    // Scanning functions (Called for specific events/objects)
    void jetdump(reco::TrackRefVector& trackRefs) const;
    void jetscan(const reco::PFJet& ijet);


    // ----------member data ---------------------------
    bool isData_;
    bool scanMode_;
    bool scanRandomJet_;

    edm::Service<TFileService> fs;
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;
    edm::EDGetTokenT<edm::View<reco::CaloJet> > jet_collT_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocVTXToken_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocCALOToken_;

    edm::ParameterSet         m_trackParameterSet;
    TrackDetectorAssociator   m_trackAssociator;
    TrackAssociatorParameters m_trackParameters;

    edm::ParameterSet         vtxconfig_;
    ConfigurableVertexReconstructor vtxmaker_;

    emjet::OutputTree otree_ ; // OutputTree object
    TTree* tree_;

    std::auto_ptr< reco::PFJetCollection > scanJet_;
    std::auto_ptr< reco::TrackCollection > scanJetTracks_;
    std::auto_ptr< reco::TrackCollection > scanJetSelectedTracks_;
    std::auto_ptr< reco::PFJetCollection > scanleadingJet_;
    std::auto_ptr< reco::TrackCollection > scanleadingJetTracks_;
    std::auto_ptr< reco::VertexCollection > avrVerticesGlobalOutput_;
    std::auto_ptr< reco::VertexCollection > avrVerticesLocalOutput_;
    std::auto_ptr< reco::TrackCollection > avrVerticesRFTracksGlobalOutput_;
    std::auto_ptr< reco::TrackCollection > avrVerticesRFTracksLocalOutput_;
    std::auto_ptr< reco::VertexCollection > darkPionVertices_;

    emjet:: Event  event_            ; // Current event
    emjet:: Jet    jet_              ; // Current jet
    emjet:: Track  track_            ; // Current track
    emjet:: Trackref trackref_       ; // Current trackref
    emjet:: Vertex vertex_           ; // Current vertex
    emjet:: GenParticle genparticle_ ; // Current genparticle
    int jet_index_         ; // Current jet index
    int track_index_       ; // Current track index
    int trackref_index_    ; // Current trackref index
    int vertex_index_      ; // Current vertex index
    int genparticle_index_ ; // Current genparticle index

    // Retrieve once per event
    // Intermediate objects used for calculations
    edm::Handle<reco::VertexCollection> primary_verticesH_;
    edm::Handle<reco::PFJetCollection> selectedJets_;
    edm::ESHandle<TransientTrackBuilder> transienttrackbuilderH_;
    std::vector<reco::TransientTrack> generalTracks_;
    edm::Handle<reco::GenParticleCollection> genParticlesH_;
    const reco::BeamSpot* theBeamSpot_;
    reco::VertexCollection selectedSecondaryVertices_;
    std::vector<TransientVertex> avrVertices_;
    edm::Handle<reco::GenJetCollection> genJets_;
    std::vector<GlobalPoint> dt_points_;
    std::vector<GlobalPoint> csc_points_;

    // Testing counters
    int pfjet;
    int pfjet_notracks;
    int pfjet_nopv;
    int pfjet_alphazero;
    int pfjet_alphaneg;
    int pfjet_alphazero_total;
    int calojet;
    int calojet_notracks;
    int calojet_nopv;
    int calojet_alphazero;
    int calojet_alphaneg;
    int calojet_alphazero_total;
};

//
// static data member definitions
//

//
// constructors and destructor
//
EmJetAnalyzer::EmJetAnalyzer(const edm::ParameterSet& iConfig):
  // event_  (new Event  ()),
  // jet_    (new Jet    ()),
  // track_  (new Track  ()),
  // vertex_ (new Vertex ())
  jet_collT_ (consumes<edm::View<reco::CaloJet> >(iConfig.getUntrackedParameter<edm::InputTag>("jets"))),
  assocVTXToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorVTX"))),
  assocCALOToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorCALO"))),
  vtxconfig_(iConfig.getParameter<edm::ParameterSet>("vertexreco")),
  vtxmaker_(vtxconfig_),
  event_       (),
  jet_         (),
  track_       (),
  trackref_    (),
  vertex_      (),
  genparticle_ ()
{
  // Config-independent initialization
  {
    // Initialize tree
    std::string modulename = iConfig.getParameter<std::string>("@module_label");
    tree_           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(tree_);
  }

  // Config-dependent initialization
  {
    // Important execution switches
    isData_ = iConfig.getParameter<bool>("isData");
    scanMode_ = iConfig.getParameter<bool>("scanMode");
    scanRandomJet_ = iConfig.getParameter<bool>("scanRandomJet");

    // Save Adaptive Vertex Reco config parameters to tree_->GetUserInfo()
    {
      double primcut = vtxconfig_.getParameter<double>("primcut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("primcut", primcut) );
      double seccut = vtxconfig_.getParameter<double>("seccut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("seccut", seccut) );
      bool smoothing = vtxconfig_.getParameter<bool>("smoothing");
      tree_->GetUserInfo()->AddLast( new TParameter<bool> ("smoothing", smoothing) );
      double minweight = vtxconfig_.getParameter<double>("minweight");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("minweight", minweight) );
    }

    m_trackParameterSet = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
    if (isData_) {
      std::cout << "running on data" << std::endl;
    } else {
      std::cout << "running on MC"   << std::endl;
    }

    edm::ConsumesCollector iC = consumesCollector();
    m_trackParameters.loadParameters( m_trackParameterSet, iC );
    m_trackAssociator.useDefaultPropagator();

    jetCollectionToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));

    // Register hard-coded inputs
    consumes<reco::BeamSpot> (edm::InputTag("offlineBeamSpot"));
    consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
    consumes<reco::TrackCollection> (edm::InputTag("displacedStandAloneMuons"));
    consumes<reco::PFMETCollection> (edm::InputTag("pfMet"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVerticesWithBS"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVertices"));
    consumes<reco::VertexCollection> (edm::InputTag("inclusiveSecondaryVertices"));

    consumes<DTRecSegment4DCollection> (edm::InputTag("dt4DSegments"));
    consumes<CSCSegmentCollection> (edm::InputTag("cscSegments"));
    consumes<RPCRecHitCollection> (edm::InputTag("rpcRecHits"));

    consumes<reco::PFCandidateCollection> (edm::InputTag("particleFlow"));

    if (!isData_) { // :MCONLY:
      consumes<std::vector<PileupSummaryInfo> > (edm::InputTag("addPileupInfo"));
      consumes<GenEventInfoProduct> (edm::InputTag("generator"));
      consumes<std::vector<reco::GenMET> > (edm::InputTag("genMetTrue"));
      consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
      consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));
    }

    // For scanning jets with alphaMax==0
    {
      produces< reco::PFJetCollection > ("scanJet"). setBranchAlias( "scanJet" ); // scanJet_
      produces< reco::TrackCollection > ("scanJetTracks"). setBranchAlias( "scanJetTracks" ); // scanJetTracks_
      produces< reco::TrackCollection > ("scanJetSelectedTracks"). setBranchAlias( "scanJetSelectedTracks" ); // scanJetSelectedTracks_
      // produces< TransientVertexCollection > ("avrVerticesGlobalOutput"). setBranchAlias( "avrVerticesGlobalOutput" ); // avrVerticesGlobalOutput_
      produces< reco::VertexCollection > ("avrVerticesGlobalOutput"). setBranchAlias( "avrVerticesGlobalOutput" ); // avrVerticesGlobalOutput_
      produces< reco::VertexCollection > ("avrVerticesLocalOutput"). setBranchAlias( "avrVerticesLocalOutput" ); // avrVerticesLocalOutput_
      produces< reco::TrackCollection > ("avrVerticesRFTracksGlobalOutput"). setBranchAlias( "avrVerticesRFTracksGlobalOutput" ); // avrVerticesRFTracksGlobalOutput_
      produces< reco::TrackCollection > ("avrVerticesRFTracksLocalOutput"). setBranchAlias( "avrVerticesRFTracksLocalOutput" ); // avrVerticesRFTracksLocalOutput_
      produces< reco::VertexCollection > ("darkPionVertices"). setBranchAlias( "darkPionVertices" ); // darkPionVertices_
    }

    //
    //For scanning leading jet
    {
      produces< reco::PFJetCollection > ("scanleadingJet"). setBranchAlias( "scanleadingJet" );// scanleadingJet_
      produces< reco::TrackCollection > ("scanleadingJetTracks"). setBranchAlias( "scanleadingJetTracks" ); // scanleadingJetTracks_
    }

  }
}


EmJetAnalyzer::~EmJetAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
EmJetAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Reset output tree to default values
  otree_.Init();
  // Reset output collections
  // Initialize output collections
  scanJet_ = std::auto_ptr< reco::PFJetCollection > ( new reco::PFJetCollection() );
  scanJetTracks_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  scanJetSelectedTracks_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  scanleadingJet_ = std::auto_ptr< reco::PFJetCollection > ( new reco::PFJetCollection() );
  scanleadingJetTracks_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  avrVerticesGlobalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  avrVerticesLocalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  avrVerticesRFTracksGlobalOutput_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  avrVerticesRFTracksLocalOutput_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  darkPionVertices_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  // Reset Event variables
  vertex_.Init();
  jet_.Init();
  event_.Init();
  genparticle_.Init();
  // Reset object counters
  jet_index_=0;
  track_index_=0;
  trackref_index_=0;
  vertex_index_=0;
  genparticle_index_=0;
  // Reset Testing counters
  pfjet = 0;
  pfjet_notracks = 0;
  pfjet_nopv = 0;
  pfjet_alphazero = 0;
  pfjet_alphaneg = 0;
  calojet = 0;
  calojet_notracks = 0;
  calojet_nopv = 0;
  calojet_alphazero = 0;
  calojet_alphaneg = 0;

  event_.run   = iEvent.id().run();
  event_.event = iEvent.id().event();
  event_.lumi  = iEvent.id().luminosityBlock();
  event_.bx    = iEvent.bunchCrossing();

  // Retrieve offline beam spot (Used to constrain vertexing)
  {
    edm::Handle<reco::BeamSpot> theBeamSpotHandle;
    iEvent.getByLabel("offlineBeamSpot", theBeamSpotHandle);
    theBeamSpot_ = theBeamSpotHandle.product();
  }

  // Calculate basic primary vertex and pileup info :EVENTLEVEL:
  {
    // iEvent.getByLabel("offlinePrimaryVerticesWithBS", primary_verticesH_);
    iEvent.getByLabel("offlinePrimaryVertices", primary_verticesH_);
    const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
    if (!isData_) { // :MCONLY: Add true number of interactions
      edm::Handle<std::vector<PileupSummaryInfo> > PileupInfo;
      iEvent.getByLabel("addPileupInfo", PileupInfo);
      for (auto const& puInfo : *PileupInfo) {
        int bx = puInfo.getBunchCrossing();
        if (bx == 0) {
          event_.nTrueInt = puInfo.getTrueNumInteractions();
        }
      }
    }
    for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
      event_.nVtx ++;
      if ( (ipv->isFake()) || (ipv->ndof() <= 4.) || (ipv->position().Rho() > 2.0) || (fabs(ipv->position().Z()) > 24.0) ) continue; // :CUT: Primary vertex cut for counting
      event_.nGoodVtx++;
    }

    // Fill primary vertex information
    VertexHigherPtSquared vertexPt2Calculator;
    double pt2sum = vertexPt2Calculator.sumPtSquared(primary_vertex);
    double nTracks = primary_vertex.tracksSize();
    event_.pv_x         = primary_vertex.x();
    event_.pv_y         = primary_vertex.y();
    event_.pv_z         = primary_vertex.z();
    event_.pv_xError    = primary_vertex.xError();
    event_.pv_yError    = primary_vertex.yError();
    event_.pv_zError    = primary_vertex.zError();
    event_.pv_chi2      = primary_vertex.chi2();
    event_.pv_ndof      = primary_vertex.ndof();
    event_.pv_pt2sum    = pt2sum;
    event_.pv_nTracks   = nTracks;
  }

  // Fill PDF information :EVENTLEVEL:
  if (!isData_) { // :MCONLY:
    edm::Handle<GenEventInfoProduct> generatorH_;
    iEvent.getByLabel("generator", generatorH_);
    if (generatorH_->hasPDF()) {
      event_.pdf_id1      = generatorH_->pdf()->id.first;
      event_.pdf_id2      = generatorH_->pdf()->id.second;
      event_.pdf_x1       = generatorH_->pdf()->x.first;
      event_.pdf_x2       = generatorH_->pdf()->x.second;
      event_.pdf_pdf1     = generatorH_->pdf()->xPDF.first;
      event_.pdf_pdf2     = generatorH_->pdf()->xPDF.second;
      event_.pdf_scalePDF = generatorH_->pdf()->scalePDF;
    }
  }

  // Retrieve event level GEN quantities
  if (!isData_) { // :MCONLY:
    // edm::Handle<std::vector<reco::GenMET> > genMetH;
    // iEvent.getByLabel("genMetTrue", genMetH);
    iEvent.getByLabel("genParticles", genParticlesH_);
    // iEvent.getByLabel("ak4GenJets",   genJets_);
    findDarkPionVertices();
  }

  // Calculate MET :EVENTLEVEL:
  {
    edm::Handle<reco::PFMETCollection> pfmet;
    iEvent.getByLabel("pfMet",pfmet);
    event_.met_pt = pfmet->begin()->pt();
    event_.met_phi = pfmet->begin()->phi();
  }

  // Retrieve selectedJets
  edm::Handle<reco::PFJetCollection> pfjetH;
  iEvent.getByToken(jetCollectionToken_, pfjetH);
  selectedJets_ = pfjetH;
  // Retrieve TransientTrackBuilder
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",transienttrackbuilderH_);
  // Retrieve generalTracks and build TransientTrackCollection
  edm::Handle<reco::TrackCollection> genTrackH;
  iEvent.getByLabel("generalTracks", genTrackH);
  // Track dump
  {
    int itk=0;
    // std::cout << "Dumping tracks\n";
    for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
      if (itk==10) break;
      reco::TrackRef trkref(genTrackH, itk);
      bool hasNonZeroWeight = false;
      for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
        double trackWeight = ipv->trackWeight(trkref);
        if (trackWeight>0) hasNonZeroWeight = true;
        // std::cout << "Track weight: " << trackWeight << std::endl;
      } // End of vertex loop
      // OUTPUT(hasNonZeroWeight);
      itk++;
    }

    edm::Handle<reco::PFCandidateCollection> pfcandidatesH;
    iEvent.getByLabel("particleFlow", pfcandidatesH);
    for ( auto cand = pfcandidatesH->begin(); cand != pfcandidatesH->end(); cand++) {
      bool matched = false;
      if (cand->charge()==0) continue;
      auto candtrkref = cand->trackRef();
      for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
        reco::TrackRef trkref(genTrackH, itk);
        if (candtrkref==trkref) matched = true;
      }
      // OUTPUT(matched);
    }
  }
  generalTracks_ = transienttrackbuilderH_->build(genTrackH);

  // Count number of tracks
  {
    event_.nTracks = generalTracks_.size();
  }

  // Reconstruct AVR vertices using all generalTracks passing basic selection
  avrVertices_.clear();
  ConfigurableVertexReconstructor avr (vtxconfig_);
  std::vector<reco::TransientTrack> tracks_for_vertexing;
  for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( selectTrack(*itk, track_) ) // :CUT: Apply basic track selection
      tracks_for_vertexing.push_back(*itk);
  }
  avrVertices_ = avr.vertices(tracks_for_vertexing);
  for (auto tv : avrVertices_) {
    avrVerticesGlobalOutput_->push_back(reco::Vertex(tv));
    if (tv.hasRefittedTracks()) {
      for (auto rftrk : tv.refittedTracks()) {
        avrVerticesRFTracksGlobalOutput_->push_back(rftrk.track());
      }
    }
  }

  // Calculate Jet-level quantities and fill into jet_ :JETLEVEL:
  for ( reco::PFJetCollection::const_iterator jet = selectedJets_->begin(); jet != selectedJets_->end(); jet++ ) {
    // Fill Jet-level quantities
    prepareJet(*jet, jet_, 1); // source = 1 for PF jets :JETSOURCE:

    // Calculate Jet-Track-level quantities and fill into jet_ :JETTRACKLEVEL:
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrackDeltaR(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 0); // source = 0 for generalTracks with simple deltaR :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 1); // source = 1 for generalTracks :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }

    // Per-jet vertex reconstruction
    {
      // Add tracks to be used for vertexing
      std::vector<reco::TransientTrack> tracks_for_vertexing;
      std::vector<reco::TransientTrack> primary_tracks;
      const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = primary_vertex.tracks_begin();
          iter != primary_vertex.tracks_end(); iter++) {
        // reco::Track trk = **iter;
        // Cast iter to TrackRef and build into TransientTrack
        reco::TransientTrack ttrk = transienttrackbuilderH_->build(iter->castTo<reco::TrackRef>());
        primary_tracks.push_back(ttrk);
      }
      for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
        if ( selectJetTrackForVertexing(*itk, jet_, track_) ) // :CUT: Apply Track selection for vertexing
          tracks_for_vertexing.push_back(*itk);
      }

      // Reconstruct vertex from tracks associated with current jet
      std::vector<TransientVertex> vertices_for_current_jet;
      {
        vertices_for_current_jet = vtxmaker_.vertices(primary_tracks, tracks_for_vertexing, *theBeamSpot_);
      }
      for (auto tv : vertices_for_current_jet) {
        avrVerticesLocalOutput_->push_back(reco::Vertex(tv));
        if (tv.hasRefittedTracks()) {
          for (auto rftrk : tv.refittedTracks()) {
            avrVerticesRFTracksLocalOutput_->push_back(rftrk.track());
          }
        }
      }
      // Fill Jet-Vertex level quantities for per-jet vertices
      for (auto vtx : vertices_for_current_jet) {
        if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
        // Fill Jet-Vertex level quantities
        prepareJetVertex(vtx, jet_, vertex_, 1); // source = 1 for per-jet AVR vertices :VERTEXSOURCE:
        {
          // Fill original tracks from current vertex
          for (auto trk : vtx.originalTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 2, iSetup);
            track_.source = 2; // source = 2 for original tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        if (vtx.hasRefittedTracks()) {
          // Fill refitted tracks from current vertex
          for (auto trk : vtx.refittedTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 3, iSetup);
            // source = 3 for refitted tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        else {
          std::cout << "No refitted tracks for current vertex!\n";
        }
        fillJetVertex(vtx, jet_, vertex_);
      }
    }

    // Fill Jet-Vertex level quantities for globally reconstructed AVR vertices
    for (auto vtx : avrVertices_) {
      if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
      // Fill Jet-Vertex level quantities
      prepareJetVertex(vtx, jet_, vertex_, 2); // source = 2 for global AVR vertices :VERTEXSOURCE:
      // Write current Vertex to Jet
      jet_.vertex_vector.push_back(vertex_);
      if (vtx.hasRefittedTracks()) {
        // Fill refitted tracks from current vertex
        for (auto trk : vtx.refittedTracks()) {
          // Fill Jet-Track level quantities
          prepareJetVertexTrack(trk, jet_, track_, vtx, 4, iSetup);
          // source = 4 for refitted tracks from global AVR vertices :TRACKSOURCE:
          // Write current Track to Jet
          jet_.track_vector.push_back(track_);
        }
      }
      else {
        std::cout << "No refitted tracks for current vertex!\n";
      }
    }
    fillJet(*jet, jet_);
    if( jet_index_==1 ){// for the leading jet
      scanleadingJet_->push_back(*jet);
      reco::TrackRefVector trackRefs = (*jet).getTrackRefs();
      for (auto tref : trackRefs) {
        scanleadingJetTracks_->push_back(*tref);
      }
    }
  }

  // Testing CALO jet association
  if (false)
  {
    edm::Handle<edm::View<reco::CaloJet> > jet_coll;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksCALO;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksVTX;
    iEvent.getByToken(jet_collT_, jet_coll);
    iEvent.getByToken(assocCALOToken_, JetTracksCALO);
    iEvent.getByToken(assocVTXToken_, JetTracksVTX);
    for(size_t i=0; i<jet_coll->size()-1; i++){
      edm::RefToBase<reco::CaloJet> jet1_ref = jet_coll->refAt(i);
      if (!(jet1_ref->pt() > 50)) continue; // :CUT: Skip CALO jets with pt < 50
      // Retrieve Jet Tracks Association
      reco::TrackRefVector dijettrks_CALO  = reco::JetTracksAssociation::getValue(*JetTracksCALO, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks_VTX  = reco::JetTracksAssociation::getValue(*JetTracksVTX, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks = MergeTracks(dijettrks_CALO, dijettrks_VTX);
      for(size_t j = 0; j<dijettrks.size(); j++){
        const reco::TrackRef trk = dijettrks[j];
        if(!trk->quality(reco::TrackBase::highPurity)) continue;
        if(trk->pt() < 5) continue;
      }
      calojet++;
      double alphaMax = compute_alphaMax(dijettrks);
      if (alphaMax==0) {
        jetdump(dijettrks);
      }

      if (primary_verticesH_->size()==0) calojet_nopv++;
      if (dijettrks.size()==0) calojet_notracks++;
      if (alphaMax==0) calojet_alphazero++;
      if (alphaMax<0) calojet_alphaneg++;
    }
  }

  if (!isData_) { // :MCONLY:
    fillGenParticles();
  }

  if (jetdump_ && pfjet_alphazero!=0 || pfjet_alphaneg!=0 || calojet_alphazero!=0 || calojet_alphaneg!=0) {
    std::cout << "Event summary:";
    OUTPUT(event_.run);
    OUTPUT(event_.lumi);
    OUTPUT(event_.event);
    OUTPUT(pfjet);
    OUTPUT(pfjet_notracks);
    OUTPUT(pfjet_nopv);
    OUTPUT(pfjet_alphazero);
    OUTPUT(pfjet_alphaneg);
    OUTPUT(calojet);
    OUTPUT(calojet_notracks);
    OUTPUT(calojet_nopv);
    OUTPUT(calojet_alphazero);
    OUTPUT(calojet_alphaneg);
    pfjet_alphazero_total += pfjet_alphazero;
    calojet_alphazero_total += calojet_alphazero;
    std::cout << "\n";
  }

  // Write current Event to OutputTree
  WriteEventToOutput(event_, &otree_);
  // Write OutputTree to TTree
  tree_->Fill();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  iEvent.put(scanJet_, "scanJet"); // scanJet_
  iEvent.put(scanJetTracks_, "scanJetTracks"); // scanJetTracks_
  iEvent.put(scanJetSelectedTracks_, "scanJetSelectedTracks"); // scanJetSelectedTracks_
  iEvent.put(scanleadingJet_, "scanleadingJet"); // scanleadingJet_
  iEvent.put(scanleadingJetTracks_, "scanleadingJetTracks"); //scanleadingJetTracks_
  iEvent.put(avrVerticesGlobalOutput_, "avrVerticesGlobalOutput"); // avrVerticesGlobalOutput_
  iEvent.put(avrVerticesLocalOutput_, "avrVerticesLocalOutput"); // avrVerticesLocalOutput_
  iEvent.put(avrVerticesRFTracksGlobalOutput_, "avrVerticesRFTracksGlobalOutput"); // avrVerticesRFTracksGlobalOutput_
  iEvent.put(avrVerticesRFTracksLocalOutput_, "avrVerticesRFTracksLocalOutput"); // avrVerticesRFTracksLocalOutput_
  iEvent.put(darkPionVertices_, "darkPionVertices"); // darkPionVertices_

  if (scanRandomJet_) return true;
  if (scanMode_) return (pfjet_alphazero>0);
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
void
EmJetAnalyzer::beginJob()
{
  pfjet_alphazero_total = 0;
  calojet_alphazero_total = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void
EmJetAnalyzer::endJob() {
  OUTPUT(pfjet_alphazero_total);
  OUTPUT(calojet_alphazero_total);
}

// ------------ method called when starting to processes a run  ------------
/*
void
EmJetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
EmJetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EmJetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EmJetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EmJetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
EmJetAnalyzer::prepareJet(const reco::PFJet& ijet, Jet& ojet, int source)
{

  pfjet++; // DEBUG
  ojet.Init();
  ojet.index = jet_index_;
  ojet.source = source;
  // Scan one jet at random
  if (scanRandomJet_ && scanJet_->size()==0) {
    if (rand() % 2 == 0) jetscan(ijet);
  }

  // Fill basic kinematic variables
  {
    ojet.pt  = ijet.pt()  ;
    ojet.eta = ijet.eta() ;
    ojet.phi = ijet.phi() ;
    ojet.p4.SetPtEtaPhiM(ojet.pt, ojet.eta, ojet.phi, 0.);
  }

  // Fill PF Jet specific variables
  {
    ojet.cef = ijet.chargedEmEnergyFraction()     ;
    ojet.nef = ijet.neutralEmEnergyFraction()     ;
    ojet.chf = ijet.chargedHadronEnergyFraction() ;
    ojet.nhf = ijet.neutralHadronEnergyFraction() ;
    ojet.pef = ijet.photonEnergyFraction()        ;
    ojet.mef = ijet.muonEnergyFraction()          ;
  }

  // Fill alphaMax
  {
    reco::TrackRefVector trackRefs = ijet.getTrackRefs();
    ojet.alphaMax = compute_alphaMax(ijet, trackRefs);
    if (ojet.alphaMax==0) {
       jetscan(ijet);
    }
    if (jetdump_ && ojet.alphaMax<=0)
      {
        std::cout << "Dumping jet\n";
        // OUTPUT(event_.run);
        // OUTPUT(event_.lumi);
        // OUTPUT(event_.event);
        // OUTPUT(event_.nTrueInt);
        OUTPUT(primary_verticesH_->size());
        OUTPUT(trackRefs.size());
        // OUTPUT(ojet.pt);
        // OUTPUT(ojet.alphaMax);
        jetdump(trackRefs);
        std::cout << "\n";
      }
    if (primary_verticesH_->size()==0) pfjet_nopv++;
    if (trackRefs.size()==0) pfjet_notracks++;
    if (ojet.alphaMax==0) pfjet_alphazero++;
    if (ojet.alphaMax<0) pfjet_alphaneg++;
  }

  // Fill nDarkPions and nDarkGluons
  {
    ojet.nDarkPions = compute_nDarkPions(ijet);
    ojet.nDarkGluons = compute_nDarkGluons(ijet);
  }

  //Fill Trackref
  {
    reco::TrackRefVector trackRefs = ijet.getTrackRefs();
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      trackref_.Init();
      trackref_.index = trackref_index_;
      trackref_.jet_index = jet_index_;
      trackref_.pt = (*ijt)->pt();
      trackref_.eta = (*ijt)->eta();
      trackref_.phi = (*ijt)->phi();
      jet_.trackref_vector.push_back(trackref_);
      trackref_index_++;
    }
  }

}

void
EmJetAnalyzer::fillJet(const reco::PFJet& ijet, Jet& ojet)
{
  // Write current Jet to Event
  event_.jet_vector.push_back(ojet);

  jet_index_++;
}

void
EmJetAnalyzer::prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source)
{
  otrack.Init();
  otrack.index = track_index_;
  otrack.source = source;
  otrack.jet_index = jet_index_;
  auto itk = &itrack;
  // Fill basic kinematic variables
  {
    otrack.pt  = itrack.track().pt()  ;
    otrack.eta = itrack.track().eta() ;
    otrack.phi = itrack.track().phi() ;
    otrack.p4.SetPtEtaPhiM(otrack.pt, otrack.eta, otrack.phi, 0.);
  }

  // Fill geometric variables
  {
    const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
    GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
    TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
    GlobalPoint closestPoint;
    if (pca.isValid()) {
      closestPoint = pca.globalPosition();
    }
    GlobalVector jetVector = direction.unit();
    Line::PositionType posJet(GlobalPoint(primary_vertex.position().x(),primary_vertex.position().y(),primary_vertex.position().z()));
    Line::DirectionType dirJet(jetVector);
    Line jetLine(posJet, dirJet);
    GlobalVector pcaToJet = jetLine.distance(closestPoint);
    TLorentzVector& trackVector = otrack.p4;
    trackVector.SetPxPyPzE(
                           closestPoint.x() - primary_vertex.position().x(),
                           closestPoint.y() - primary_vertex.position().y(),
                           closestPoint.z() - primary_vertex.position().z(),
                           itk->track().p());
    otrack.dRToJetAxis = trackVector.DeltaR(ojet.p4);
    // Calculate PCA coordinates
    otrack.pca_r   = closestPoint.perp();
    otrack.pca_eta = closestPoint.eta();
    otrack.pca_phi = closestPoint.phi();
    otrack.distanceToJet = pcaToJet.mag();

    auto dxy_ipv = IPTools::absoluteTransverseImpactParameter(*itk, primary_vertex);
    otrack.ipXY    = fabs(dxy_ipv.second.value());
    otrack.ipXYSig = fabs(dxy_ipv.second.significance());
  }

  otrack.quality             = itk->track().qualityMask();
  otrack.algo                = itk->track().algo();
  otrack.originalAlgo        = itk->track().originalAlgo();
  otrack.nHits               = itk->numberOfValidHits();
  otrack.nMissInnerHits      = itk->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nTrkLayers          = itk->hitPattern().trackerLayersWithMeasurement();
  otrack.nMissTrkLayers      = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
  otrack.nPxlLayers          = itk->hitPattern().pixelLayersWithMeasurement();
  otrack.nMissPxlLayers      = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
}

void
EmJetAnalyzer::fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack)
{
  // Write current Track to Jet
  jet_.track_vector.push_back(track_);

  track_index_++;
}


void
EmJetAnalyzer::prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup)
{
  static CheckHitPattern checkHitPattern;

  prepareJetTrack(itrack, ojet, otrack, source);
  otrack.vertex_index = vertex_index_;
  otrack.vertex_weight = ivertex.trackWeight(itrack);
  bool fixHitPattern = true;
  CheckHitPattern::Result hitInfo = checkHitPattern.analyze(iSetup, itrack.track(), ivertex.vertexState(), fixHitPattern);
  otrack.nHitsInFrontOfVert = hitInfo.hitsInFrontOfVert;
  otrack.missHitsAfterVert  = hitInfo.missHitsAfterVert;
}

void
EmJetAnalyzer::prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source)
{
  overtex.Init();
  overtex.index = vertex_index_;
  overtex.source = source;
  overtex.jet_index = jet_index_;

  auto vtx = reco::Vertex(ivertex);
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  double Lxy = 0;
  float dx = primary_vertex.position().x() - vtx.position().x();
  float dy = primary_vertex.position().y() - vtx.position().y();
  Lxy = TMath::Sqrt( dx*dx + dy*dy );
  float mass = vtx.p4().mass();
  float pt2sum = compute_pt2Sum(ivertex);
  overtex.x      = ( vtx.x()      );
  overtex.y      = ( vtx.y()      );
  overtex.z      = ( vtx.z()      );
  overtex.xError = ( vtx.xError() );
  overtex.yError = ( vtx.yError() );
  overtex.zError = ( vtx.zError() );
  overtex.deltaR = ( deltaR       );
  overtex.Lxy    = ( Lxy          );
  overtex.mass   = ( mass         );
  overtex.chi2   = ( vtx.chi2()   );
  overtex.ndof   = ( vtx.ndof()   );
  overtex.pt2sum = ( pt2sum       );
}

void
EmJetAnalyzer::fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex)
{
  // Write current Vertex to Jet
  jet_.vertex_vector.push_back(vertex_);

  vertex_index_++;
}

bool
EmJetAnalyzer::selectTrack(const reco::TransientTrack& itrack, const Track& otrack)
{
  auto itk = &itrack;
  // Skip tracks with pt<1 :CUT:
  if (itk->track().pt() < 1.) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack)
{
  auto itk = &itrack;
  if (!selectTrack(itrack, otrack)) return false; // :CUT: Require track to pass basic selection

  // Skip tracks with invalid point-of-closest-approach :CUT:
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
  TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
  if (!pca.isValid()) return false;
  GlobalPoint closestPoint = pca.globalPosition();

  // Skip tracks if point-of-closest-approach has -nan or nan x/y/z coordinates :CUT:
  if ( !( std::isfinite(closestPoint.x()) && std::isfinite(closestPoint.y()) && std::isfinite(closestPoint.z()) ) ) return false;

  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  TLorentzVector trackVector;
  trackVector.SetPxPyPzE(
                         closestPoint.x() - primary_vertex.position().x(),
                         closestPoint.y() - primary_vertex.position().y(),
                         closestPoint.z() - primary_vertex.position().z(),
                         itk->track().p());
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrackDeltaR(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack)
{
  auto itk = &itrack;
  if (!selectTrack(itrack, otrack)) return false; // :CUT: Require track to pass basic selection

  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  TLorentzVector trackVector;
  trackVector.SetPxPyPzE(
                         itk->track().px(),
                         itk->track().py(),
                         itk->track().pz(),
                         itk->track().p());
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;

  return true;
}

bool
EmJetAnalyzer::selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack)
{
  return selectJetTrackDeltaR(itrack, ojet, otrack);
}

bool
EmJetAnalyzer::selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex)
{
  auto vtx = reco::Vertex(ivertex);

  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  if (deltaR > 0.4) return false; // Ignore vertices outside jet cone :CUT:
  return true;
}

void
EmJetAnalyzer::fillGenParticles () {
  for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
    const reco::Candidate* cand = &(*gp);
    genparticle_.status = gp->status();
    genparticle_.pdgId = gp->pdgId();
    genparticle_.charge = gp->charge();
    genparticle_.mass = gp->mass();
    genparticle_.pt = gp->pt();
    genparticle_.eta = gp->eta();
    genparticle_.phi = gp->phi();
    genparticle_.vx = gp->vx();
    genparticle_.vy = gp->vy();
    genparticle_.vz = gp->vz();
    double Lxy = -1;
    if ( cand->numberOfDaughters()>0 ) {
      const reco::Candidate* dau = cand->daughter(0);
      if (dau) {
        Lxy = TMath::Sqrt( dau->vx()*dau->vx() + dau->vy()*dau->vy() );
      }
    }
    genparticle_.Lxy = Lxy;
    genparticle_.isDark = GenParticleAnalyzer::isDark(cand);
    genparticle_.nDaughters = cand->numberOfDaughters();
    bool hasSMDaughter = false;
    if ( (GenParticleAnalyzer::hasDarkDaughter(cand) == false) && (cand->numberOfDaughters()>1) ) hasSMDaughter = true;
    genparticle_.hasSMDaughter = hasSMDaughter;
    genparticle_.hasDarkMother = GenParticleAnalyzer::hasDarkMother(cand);
    genparticle_.hasDarkPionMother = GenParticleAnalyzer::hasDarkPionMother(cand);
    // if (genparticle_.hasDarkMother && !genparticle_.isDark) std::cout<< "SM particle with Dark mother - status: " << genparticle_.status << "\t pdgId: " <<  (genparticle_.pdgId) << "\t nDau: " << gp->numberOfDaughters() << std::endl;
    bool isTrackable = false;
    int nTrackableDaughters = 0;
    for ( unsigned int i = 0; i != cand->numberOfDaughters(); ++i){
      auto dau = cand->daughter(i);
      if (dau->pt() > 1 && dau->charge()!=0 && !GenParticleAnalyzer::isDark(dau)) nTrackableDaughters++;
    }
    if (nTrackableDaughters>1) isTrackable=true;
    genparticle_.isTrackable = isTrackable;
    // if (cand->pdgId()==4900111) {GenParticleAnalyzer::printParticleMothers(cand->daughter(0));}
    // if (cand->numberOfMothers()>2) {
    //   std::cout<<"GP with more than two mothers\n";
    //   OUTPUT(cand->pdgId());
    //   OUTPUT(cand->status());
    //   OUTPUT(cand->pt());
    //   OUTPUT(cand->numberOfMothers());
    //   GenParticleAnalyzer::printParticleMothers(cand);
    // }
    float min2Ddist = 999999.;
    float min2Dsig  = 999999.;
    float min3Ddist = 999999.;
    float min3Dsig  = 999999.;
    float minDeltaR = 999999.;
    float matched2Ddist = 999999.;
    float matched2Dsig  = 999999.;
    float matched3Ddist = 999999.;
    float matched3Dsig  = 999999.;
    float matchedDeltaR = 999999.;
    // GenParticle vx/vy/vz returns production vertex position, so use first daughter to find decay vertex if it exists
    if (cand->numberOfDaughters()>0) {
      auto decay = cand->daughter(0);
      for (auto vtx: avrVertices_) {
        float dx = decay->vx() - vtx.position().x();
        float dy = decay->vy() - vtx.position().y();
        float dz = decay->vz() - vtx.position().z();
        float exx = vtx.positionError().cxx();
        float eyy = vtx.positionError().cyy();
        float ezz = vtx.positionError().czz();
        float dist2D = TMath::Sqrt( dx*dx + dy*dy );
        float dist3D = TMath::Sqrt( dx*dx + dy*dy + dz*dz );
        float error2D = TMath::Sqrt( exx + eyy );
        float error3D = TMath::Sqrt( exx + eyy + ezz );
        float sig2D = dist2D/error2D;
        float sig3D = dist3D/error3D;
        TLorentzVector decayVector (decay->vx(), decay->vy(), decay->vz(), 0.);
        TLorentzVector vtxVector (vtx.position().x(), vtx.position().y(), vtx.position().z(), 0.);
        float deltaR = decayVector.DeltaR(vtxVector);
        if (dist2D<min2Ddist) {
          min2Ddist = dist2D;
          matched2Ddist = dist2D;
          matched2Dsig = sig2D;
          matched3Ddist = dist3D;
          matched3Dsig = sig3D;
          matchedDeltaR = deltaR;
        }
        if (dist3D<min3Ddist) min3Ddist = dist3D;
        if (sig2D<min2Dsig) min2Dsig = sig2D;
        if (sig3D<min3Dsig) min3Dsig = sig3D;
        if (deltaR<minDeltaR) minDeltaR = deltaR;
      }
    }
    genparticle_.min2Ddist = min2Ddist;
    genparticle_.min2Dsig  = min2Dsig;
    genparticle_.min3Ddist = min3Ddist;
    genparticle_.min3Dsig  = min3Dsig;
    genparticle_.minDeltaR = minDeltaR;
    genparticle_.matched2Ddist = matched2Ddist;
    genparticle_.matched2Dsig  = matched2Dsig;
    genparticle_.matched3Ddist = matched3Ddist;
    genparticle_.matched3Dsig  = matched3Dsig;
    genparticle_.matchedDeltaR = matchedDeltaR;

    event_.genparticle_vector.push_back(genparticle_);
    genparticle_.index++;
  }
}

void
EmJetAnalyzer::findDarkPionVertices () {
  for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
    if (gp->pdgId()==4900111) {
      auto dau = gp->daughter(0);
      double x = dau->vx();
      double y = dau->vy();
      double z = dau->vz();
      reco::Vertex::Error e;
      // e(0, 0) = 0.0015 * 0.0015;
      // e(1, 1) = 0.0015 * 0.0015;
      // e(2, 2) = 15. * 15.;
      reco::Vertex::Point p(x, y, z);
      reco::Vertex dpVertex(p, e, 0, 0, 0);
      darkPionVertices_->push_back(dpVertex);
    }
  }
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  //std::cout<< "***********************************************************" << std::endl;
  //std::cout<<"************************************************************" << std::endl;
  //std::cout<< "jet pt: "<< ijet.pt() << " jet eta: "<< ijet.eta() << " jet phi: "<< ijet.phi() << std::endl;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  //std::cout<< "trackRefs size: " << trackRefs.size() <<  " jet_pt_sum: "<< jet_pt_sum << std::endl;

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    //std::cout<< "+++++++++++++++++++++++" << std::endl;
    //std::cout<< "primary vertex: " << std::setw(15) << ipv->x() << std::setw(15) << ipv->y() << std::setw(15) << ipv->z() << std::endl;
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      //std::cout << "track pt "<< (*ijt)->pt() << " track weight " << trackWeight<< std::endl;
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    //std::cout << "vertex_pt_sum "<< vertex_pt_sum << std::endl;
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  //std::cout<< "jet pt "<< ijet.pt() << " max_vertex_pt_sum: "<< max_vertex_pt_sum << " jet_pt_sum "<< jet_pt_sum << " alphaMax "<< alphaMax << std::endl;
  return alphaMax;
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(reco::TrackRefVector& trackRefs) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

int
EmJetAnalyzer::compute_nDarkPions(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900111) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

int
EmJetAnalyzer::compute_nDarkGluons(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900021) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

void
EmJetAnalyzer::jetdump(reco::TrackRefVector& trackRefs) const
{
  // auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  // double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  // std::cout << "Track qualities:\n";
  // for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::highPurity));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::loose));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::tight));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::goodIterative));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::highPurity));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::loose));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::tight));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::goodIterative));
  // }
  // for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
  //   std::cout << "New vertex\n";
  //   for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //     double trackWeight = ipv->trackWeight(*ijt);
  //     std::cout << "Track weight: " << trackWeight << std::endl;
  //   } // End of track loop
  // } // End of vertex loop
}

void
EmJetAnalyzer::jetscan(const reco::PFJet& ijet) {
  reco::TrackRefVector trackRefs = ijet.getTrackRefs();
  scanJet_->push_back(ijet);
  for (auto tref : trackRefs) {
    scanJetTracks_->push_back(*tref);
  }
  for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
    scanJetSelectedTracks_->push_back(itk->track());
  }
}

double
EmJetAnalyzer::compute_pt2Sum (const TransientVertex& ivertex) const {
  auto vtx = reco::Vertex(ivertex);
  // Modified from reco::Vertex::p4()
  double sum = 0.;
  double pt = 0.;
  if(vtx.hasRefittedTracks()) {
    for(std::vector<reco::Track>::const_iterator iter = vtx.refittedTracks().begin();
        iter != vtx.refittedTracks().end(); ++iter) {
      pt = iter->pt();
      sum += pt*pt;
    }
  }
  else
    {
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = vtx.tracks_begin();
          iter != vtx.tracks_end(); iter++) {
        pt = (*iter)->pt();
        sum += pt*pt;
      }
    }
  return sum;
}

// Merge two TrackRefVector objects
reco::TrackRefVector
EmJetAnalyzer::MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2){
  reco::TrackRefVector mergedtrks;
  mergedtrks = trks1;
  for(size_t i = 0; i<trks2.size(); i++){
    bool new_trk = true;
    for(size_t j = 0; j< trks1.size(); j++){
      if(trks2[i].index()==trks1[j].index()){
        new_trk = false;
      }
    }
    if(new_trk) mergedtrks.push_back(trks2[i]);
  }
  return mergedtrks;

}


//define this as a plug-in
DEFINE_FWK_MODULE(EmJetAnalyzer);
