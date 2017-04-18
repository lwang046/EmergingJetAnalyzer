// -*- C++ -*-
//
// Package:    MyAnalysis/EmergingJetAnalyzer
// Class:      EmergingJetAnalyzer
//
/**\class EmergingJetAnalyzer EmergingJetAnalyzer.cc MyAnalysis/EmergingJetAnalyzer/plugins/EmergingJetAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Ted Ritchie Kolberg
//         Created:  Tue, 27 Jan 2015 12:30:51 GMT
//
//

#define VERTEXRECOTESTING 0


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/METReco/interface/METFwd.h"
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

#include "RecoVertex/AdaptiveVertexFinder/interface/AdaptiveVertexReconstructor.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/JetReco/interface/GenJet.h"

// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

#include "EmergingJetAnalysis/EmergingJetAnalyzer/interface/OutputTree.h"
//
// class declaration
//

class EmergingJetAnalyzer : public edm::EDAnalyzer {
  public:
    explicit EmergingJetAnalyzer(const edm::ParameterSet&);
    ~EmergingJetAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


  private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    // ---------- helper functions ---------------------------
    // Take a single PFJet and add to output tree
    void fillSingleJet(const reco::PFJet&, int jet_index);
    void fillVertexForSingleJet(const reco::VertexCollection&, int source);
    // Select all secondary vertices passing certain criteria, from a given vertexCollection
    reco::VertexCollection selectSecondaryVertices (edm::Handle<reco::VertexCollection>) const;
    // Calculate sum of pt-squares of all tracks, for a given vertex
    double calculatePt2Sum (const reco::Vertex&) const;

    TrackDetectorAssociator   m_trackAssociator;
    TrackAssociatorParameters m_trackParameters;
    edm::ParameterSet         m_trackParameterSet;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;
    edm::EDGetTokenT< reco::METCollection > HtMhtCollectionToken_;
    edm::EDGetTokenT< reco::BeamSpot > beamSpotToken_;

    emjet::OutputTree otree_;

    // Retrieve once per event
    // Intermediate objects used for calculations
    edm::ESHandle<TransientTrackBuilder> transienttrackbuilderH_;
    edm::Handle<reco::VertexCollection> primary_verticesH_;
    std::vector<reco::TransientTrack> generalTracks_;
    edm::Handle<reco::GenParticleCollection> genParticlesH_;
    const reco::BeamSpot* theBeamSpot_;
    reco::VertexCollection selectedSecondaryVertices_;
    std::vector<TransientVertex> avrVertices_;
    edm::Handle<reco::GenJetCollection> genJets_;
    std::vector<GlobalPoint> dt_points_;
    std::vector<GlobalPoint> csc_points_;

    // Calculate once per jet
    TLorentzVector jetVector_;

    bool isData_;

    TTree* t_tree;

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
EmergingJetAnalyzer::EmergingJetAnalyzer(const edm::ParameterSet& iConfig) :
  m_trackParameterSet(iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters")),
  isData_(iConfig.getUntrackedParameter<bool>("isData"))
{
  //now do what ever initialization is needed

  if (isData_) {
    std::cout << "running on data" << std::endl;
  } else {
    std::cout << "running on MC"   << std::endl;
  }

  edm::ConsumesCollector iC = consumesCollector();
  m_trackParameters.loadParameters( m_trackParameterSet, iC );
  m_trackAssociator.useDefaultPropagator();

  jetCollectionToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));

  // Register inputs
  consumes<reco::BeamSpot> (edm::InputTag("offlineBeamSpot"));
  consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
  consumes<reco::TrackCollection> (edm::InputTag("displacedStandAloneMuons"));
  consumes<reco::PFMETCollection> (edm::InputTag("pfMet"));
  consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVerticesWithBS"));
  consumes<reco::VertexCollection> (edm::InputTag("inclusiveSecondaryVertices"));

  consumes<DTRecSegment4DCollection> (edm::InputTag("dt4DSegments"));
  consumes<CSCSegmentCollection> (edm::InputTag("cscSegments"));
  consumes<RPCRecHitCollection> (edm::InputTag("rpcRecHits"));

  if (!isData_) { // :MCONLY:
    consumes<std::vector<PileupSummaryInfo> > (edm::InputTag("addPileupInfo"));
    consumes<std::vector<reco::GenMET> > (edm::InputTag("genMetTrue"));
    consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
    consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));
  }

  t_tree           = fs->make<TTree>("emergingJetsTree","emergingJetsTree");
  otree_.Branch(t_tree);

}


EmergingJetAnalyzer::~EmergingJetAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
  void
EmergingJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  // Reset output tree to default values
  otree_.Init();

  //   float ipCut = 0.05;

  edm::ESHandle<GlobalTrackingGeometry> theTrackingGeometry;
  iSetup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry);

  // edm::ESHandle<TransientTrackBuilder> transienttrackbuilderH_;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",transienttrackbuilderH_);

  edm::Handle<reco::PFJetCollection> pfjetH;
  // iEvent.getByLabel("ak4PFJetsCHS", pfjetH);
  iEvent.getByToken(jetCollectionToken_, pfjetH);
  auto selectedJets = *pfjetH;

  Handle<reco::BeamSpot> theBeamSpotHandle;
  iEvent.getByLabel("offlineBeamSpot", theBeamSpotHandle);
  theBeamSpot_ = theBeamSpotHandle.product();

  //   edm::Handle<reco::CaloJetCollection> caloJetH;
  //   iEvent.getByLabel("ak5CaloJets", caloJetH);

  edm::Handle<reco::TrackCollection> genTrackH;
  iEvent.getByLabel("generalTracks", genTrackH);
  generalTracks_ = transienttrackbuilderH_->build(genTrackH);

  edm::Handle<reco::TrackCollection> sdmH;
  iEvent.getByLabel("displacedStandAloneMuons",sdmH);
  std::vector<reco::TransientTrack> standaloneDisplacedMuons;
  standaloneDisplacedMuons = transienttrackbuilderH_->build(sdmH);

  edm::Handle<reco::PFMETCollection> pfmet;
  iEvent.getByLabel("pfMet",pfmet);

  iEvent.getByLabel("offlinePrimaryVerticesWithBS",primary_verticesH_);
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);

  edm::Handle<reco::VertexCollection> secondary_vertices;
  iEvent.getByLabel("inclusiveSecondaryVertices",secondary_vertices);

  if (!isData_) { // :MCONLY: Add true number of interactions
    edm::Handle<std::vector<PileupSummaryInfo> > PileupInfo;
    iEvent.getByLabel("addPileupInfo", PileupInfo);

    for (auto const& puInfo : *PileupInfo) {
      int bx = puInfo.getBunchCrossing();
      if (bx == 0) {
        otree_.nTrueInt = puInfo.getTrueNumInteractions();
      }
    }
  }

  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    otree_.nVtx ++;
    if ( (ipv->isFake()) || (ipv->ndof() <= 4.) || (ipv->position().Rho() > 2.0) || (fabs(ipv->position().Z()) > 24.0) ) continue; // :CUT: Primary vertex cut for counting
    otree_.nGoodVtx++;
  }

  ////////////////////////////////////////////////////////////
  // Reconstruct vertices from scratch
  ////////////////////////////////////////////////////////////
  {
    avrVertices_.clear();
    AdaptiveVertexReconstructor avr (2.0, 6.0, 0.5, true );
    avrVertices_ = avr.vertices(generalTracks_);
  }
  if (VERTEXRECOTESTING)
  {
    TStopwatch timer;
    timer.Start();
    std::cout << "Running KalmanTrimmedVertexFinder" << std::endl;
    KalmanTrimmedVertexFinder finder;
    finder.setPtCut(0.0);
    finder.setTrackCompatibilityCut(0.05);
    finder.setTrackCompatibilityToSV(0.01);
    finder.setVertexFitProbabilityCut(0.01);
    finder.setMaxNbOfVertices(0);
    vector<TransientVertex> vertices = finder.vertices ( generalTracks_ );
    std::cout << "Number of KTVF vertices: " << vertices.size() << std::endl;
    int nSV = 0;
    for (TransientVertex vertex: vertices) {
      auto vtx = reco::Vertex(vertex);
      double Lxy = 0;
      float dx = primary_vertex.position().x() - vtx.position().x();
      float dy = primary_vertex.position().y() - vtx.position().y();
      Lxy = TMath::Sqrt( dx*dx + dy*dy );
      if (Lxy>1) nSV++;
    }
    std::cout << "Number of displaced KTVF vertices: " << nSV << std::endl;
    std::cout << "Time elapsed:" << timer.RealTime() << std::endl;

    timer.Start();
    std::cout << "Running AdaptiveVertexReconstructor" << std::endl;
    AdaptiveVertexReconstructor avr (2.0, 6.0, 0.5, true );
    std::vector<TransientVertex> theVertices = avr.vertices(generalTracks_);
    std::cout << "Number of AVR vertices: " << theVertices.size() << std::endl;
    nSV = 0;
    for (TransientVertex vertex: theVertices) {
      auto vtx = reco::Vertex(vertex);
      double Lxy = 0;
      float dx = primary_vertex.position().x() - vtx.position().x();
      float dy = primary_vertex.position().y() - vtx.position().y();
      Lxy = TMath::Sqrt( dx*dx + dy*dy );
      if (Lxy>1) nSV++;
    }
    std::cout << "Number of displaced AVR vertices: " << nSV << std::endl;
    std::cout << "Time elapsed:" << timer.RealTime() << std::endl;

    nSV = 0;
    for (size_t ivx = 0; ivx < secondary_vertices->size(); ++ivx) {
      auto vtx = secondary_vertices->at(ivx);
      double Lxy = 0;
      float dx = primary_vertex.position().x() - vtx.position().x();
      float dy = primary_vertex.position().y() - vtx.position().y();
      Lxy = TMath::Sqrt( dx*dx + dy*dy );
      if (Lxy>1) nSV++;
    }
    std::cout << "Number of displaced vertices in inclusiveSecondaryVertices: " << nSV << std::endl;
  }


  // Fill event level GEN quantities
  if (!isData_) {
    edm::Handle<std::vector<reco::GenMET> > genMetH;
    iEvent.getByLabel("genMetTrue", genMetH);
    iEvent.getByLabel("genParticles", genParticlesH_);
    iEvent.getByLabel("ak4GenJets",   genJets_);

  }

  otree_.run   = iEvent.id().run();
  otree_.event = iEvent.id().event();
  otree_.lumi  = iEvent.id().luminosityBlock();
  otree_.bx    = iEvent.bunchCrossing();

  // Do stuff with muon system
  // Save dt_points_
  // Save csc_points_
  {
    Handle<DTRecSegment4DCollection> segments4D;
    iEvent.getByLabel("dt4DSegments",segments4D);

    GlobalPoint segpos;
    dt_points_.clear();
    for (auto iseg = segments4D->begin(); iseg != segments4D->end(); ++iseg) {
      const GeomDet* geomDet = theTrackingGeometry->idToDet((*iseg).geographicalId());
      segpos = geomDet->toGlobal((*iseg).localPosition());
      dt_points_.push_back(segpos);
    }

    Handle<CSCSegmentCollection> csc_segments;
    iEvent.getByLabel("cscSegments",csc_segments);

    csc_points_.clear();
    for (auto icsc = csc_segments->begin(); icsc != csc_segments->end(); ++icsc) {
      const GeomDet* geomDet = theTrackingGeometry->idToDet((*icsc).geographicalId());
      segpos = geomDet->toGlobal((*icsc).localPosition());
      csc_points_.push_back(segpos);
    }

    Handle<RPCRecHitCollection> rpc_hits;
    iEvent.getByLabel("rpcRecHits",rpc_hits);
  }

  otree_.met_pt = pfmet->begin()->pt();
  otree_.met_phi = pfmet->begin()->phi();

  // Select secondary vertices
  selectedSecondaryVertices_ = selectSecondaryVertices(secondary_vertices);
  // Loop over secondary vertices
  for (auto vtx : selectedSecondaryVertices_) {
    float Lxy = 0.;
    float mass = 0.;
    float pt2sum = 0.;
    float dx = primary_vertex.position().x() - vtx.position().x();
    float dy = primary_vertex.position().y() - vtx.position().y();
    Lxy = TMath::Sqrt( dx*dx + dy*dy );
    mass = vtx.p4().mass();
    pt2sum = calculatePt2Sum(vtx);
    // bool hasRefittedTracks = vtx.hasRefittedTracks();
    // std::cout << "pt2sum: " << pt2sum << std::endl;
    // std::cout << "hasRefittedTracks: " << hasRefittedTracks << std::endl;
    otree_.vertex_x      .push_back( vtx.x()                   );
    otree_.vertex_y      .push_back( vtx.y()                   );
    otree_.vertex_z      .push_back( vtx.z()                   );
    otree_.vertex_xError .push_back( vtx.xError()              );
    otree_.vertex_yError .push_back( vtx.yError()              );
    otree_.vertex_zError .push_back( vtx.zError()              );
    otree_.vertex_Lxy    .push_back( Lxy                       );
    otree_.vertex_mass   .push_back( mass                      );
    otree_.vertex_chi2   .push_back( vtx.chi2()                );
    otree_.vertex_ndof   .push_back( vtx.ndof()                );
    otree_.vertex_pt2sum .push_back( pt2sum                    );
  }

  ////////////////////////////////////////////////////////////
  // selectedJet Loop begin
  ////////////////////////////////////////////////////////////
  int ijet=0;
  for ( reco::PFJetCollection::const_iterator jet = selectedJets.begin(); jet != selectedJets.end(); ++jet ) {
    fillSingleJet(*jet, ijet);
    ++ijet;
  }
  ////////////////////////////////////////////////////////////
  // selectedJet Loop end
  ////////////////////////////////////////////////////////////

  t_tree->Fill();

  return;



}


// ------------ method called once each job just before starting event loop  ------------
  void
EmergingJetAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
  void
EmergingJetAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   EmergingJetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   EmergingJetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   EmergingJetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   EmergingJetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EmergingJetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
EmergingJetAnalyzer::fillSingleJet(const reco::PFJet& jet, int jet_index) {
  // Shared objects
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  jetVector_.SetPtEtaPhiM(jet.pt(),jet.eta(),jet.phi(),0.);
  // std::cout << "jet.pt(): " << jet.pt() << std::endl;
  const float maxSigPromptTrack = 3.;
  const float minSigDispTrack = 3.;

  // Calculate nPromptTracks
  int nPromptTracks = 0;
  {
    reco::TrackRefVector trackRefs = jet.getTrackRefs();
    // Loop over tracks belonging to jet and calculate nPromptTracks
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      reco::TransientTrack itk = transienttrackbuilderH_->build(*ijt);
      if (itk.track().pt() < 1.) continue;
      auto d3d_ipv = IPTools::absoluteImpactParameter3D(itk, primary_vertex);
      if (d3d_ipv.second.significance() < maxSigPromptTrack) nPromptTracks++;
      //       std::cout << "Track with value, significance " << dxy_ipv.second.value() << "\t" << dxy_ipv.second.significance() << std::endl;
      //           if (dxy_ipv.second.value() > ipCut) continue;
    }
  }

  // Calculate nDispTracks, medianLogIpSig, fill track variables
  int nDispTracks   = 0;
  float medianLogIpSig = 0.;
  int nTags = 0;
  const float logTagCut    = 2.;
  // per track variables, reuse for efficiency
  std:: vector<int>   vec_source              ;
  std:: vector<float> vec_pt                  ;
  std:: vector<float> vec_eta                 ;
  std:: vector<float> vec_phi                 ;
  std:: vector<float> vec_pca_r               ;
  std:: vector<float> vec_pca_eta             ;
  std:: vector<float> vec_pca_phi             ;
  std:: vector<int>   vec_algo                ;
  std:: vector<int>   vec_originalAlgo        ;
  std:: vector<int>   vec_nHits               ;
  std:: vector<int>   vec_nMissInnerHits      ;
  std:: vector<int>   vec_nTrkLayers          ;
  std:: vector<int>   vec_nMissTrkLayers      ;
  std:: vector<int>   vec_nMissInnerTrkLayers ;
  std:: vector<int>   vec_nMissOuterTrkLayers ;
  std:: vector<int>   vec_nPxlLayers          ;
  std:: vector<int>   vec_nMissPxlLayers      ;
  std:: vector<int>   vec_nMissInnerPxlLayers ;
  std:: vector<int>   vec_nMissOuterPxlLayers ;
  std:: vector<float> vec_ipXY                ;
  std:: vector<float> vec_ipZ                 ;
  std:: vector<float> vec_ipXYSig             ;
  std:: vector<float> vec_dRToJetAxis         ;
  std:: vector<float> vec_distanceToJet       ;
  std:: vector<float> vec_vertexLxy           ;
  int itrack = 0;
  {
    vec_source              .clear();
    vec_pt                  .clear();
    vec_eta                 .clear();
    vec_phi                 .clear();
    vec_pca_r               .clear();
    vec_pca_eta             .clear();
    vec_pca_phi             .clear();
    vec_algo                .clear();
    vec_originalAlgo        .clear();
    vec_nHits               .clear();
    vec_nMissInnerHits      .clear();
    vec_nTrkLayers          .clear();
    vec_nMissTrkLayers      .clear();
    vec_nMissInnerTrkLayers .clear();
    vec_nMissOuterTrkLayers .clear();
    vec_nPxlLayers          .clear();
    vec_nMissPxlLayers      .clear();
    vec_nMissInnerPxlLayers .clear();
    vec_nMissOuterPxlLayers .clear();
    vec_ipXY                .clear();
    vec_ipZ                 .clear();
    vec_ipXYSig             .clear();
    vec_dRToJetAxis         .clear();
    vec_distanceToJet       .clear();
    vec_vertexLxy           .clear();

    std::vector<float> ipVector;
    float pTxIPxSig = 0.;
    std::vector<reco::TransientTrack> ipTracks;
    float trackFrac = 0.;

    math::XYZVector jetMomentum = jet.momentum();
    GlobalVector direction(jetMomentum.x(), jetMomentum.y(), jetMomentum.z());

    int misshits = 0;
    float tot_dsz = 0.;
    std::vector<float> r_pca;
    std::vector<float> logIpSig;
    float sum_Lxy = 0.;
    // std::cout << "generalTracks_.size(): " << generalTracks_.size() << std::endl;
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {

      // Skip tracks with pt<1
      if (itk->track().pt() < 1.) continue;
      itrack++;

      auto dxy_ipv = IPTools::absoluteTransverseImpactParameter(*itk, primary_vertex);
      float ipXY = fabs(dxy_ipv.second.value());
      float ipXYSig = fabs(dxy_ipv.second.significance());

      //       std::cout << "Track with value, significance " << dxy_ipv.second.value() << "\t" << dxy_ipv.second.significance() << std::endl;

      TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex,
          direction, itk->field());

      // Skip tracks with invalid point-of-closest-approach :CUT:
      GlobalPoint closestPoint;
      if (pca.isValid()) {
        closestPoint = pca.globalPosition();
      } else {
        continue;
      }
      // Calculate PCA coordinates
      double pca_r   = closestPoint.perp();
      double pca_eta = closestPoint.eta();
      double pca_phi = closestPoint.phi();
      // Skip tracks if point-of-closest-approach has -nan or nan x/y/z coordinates :CUT:
      if ( ! ( std::isfinite(closestPoint.x()) && std::isfinite(closestPoint.y()) && std::isfinite(closestPoint.z()) ) )
        continue;
      // Calculate jet-to-track distance using pca
      // See IPTools::jetTrackDistance() for reference
      // Assumes the jet originates from primary_vertex
      // Construct the jet line
      GlobalVector jetVector = direction.unit();
      Line::PositionType posJet(GlobalPoint(primary_vertex.position().x(),primary_vertex.position().y(),primary_vertex.position().z()));
      Line::DirectionType dirJet(jetVector);
      Line jetLine(posJet,dirJet);
      GlobalVector pcaToJet = jetLine.distance(closestPoint);
      double distanceToJet = pcaToJet.mag();

      TLorentzVector trackVector;
      trackVector.SetPxPyPzE(
          closestPoint.x() - primary_vertex.position().x(),
          closestPoint.y() - primary_vertex.position().y(),
          closestPoint.z() - primary_vertex.position().z(),
          itk->track().p());


      // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
      float deltaR = trackVector.DeltaR(jetVector_);
      // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
      if (deltaR > 0.4) continue;
      float dRToJetAxis = deltaR;

      int source = 0; // 0 for generalTracks
      double pt    = itk->track().pt();
      double eta   = itk->track().eta();
      double phi   = itk->track().phi();
      int algo     = itk->track().algo();
      int originalAlgo = itk->track().originalAlgo();
      int nHits               = itk->numberOfValidHits();
      int nMissInnerHits      = itk->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
      int nTrkLayers          = itk->hitPattern().trackerLayersWithMeasurement();
      int nMissTrkLayers      = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
      int nMissInnerTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
      int nMissOuterTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
      int nPxlLayers          = itk->hitPattern().pixelLayersWithMeasurement();
      int nMissPxlLayers      = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
      int nMissInnerPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
      int nMissOuterPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
      double vertexLxy = -1; // For generalTracks
      // std::cout << "nHits:" << nHits << std::endl;
      // std::cout << "nMissInnerHits:" << nMissInnerHits << std::endl;
      /*
       */
#define VEC_PUSHBACK(a) vec_##a.push_back(a)
      // Pushback variable into vec_<VARIABLENAME>
      VEC_PUSHBACK( source              );
      VEC_PUSHBACK( pt                  );
      VEC_PUSHBACK( eta                 );
      VEC_PUSHBACK( phi                 );
      VEC_PUSHBACK( algo                );
      VEC_PUSHBACK( pca_r               );
      VEC_PUSHBACK( pca_eta             );
      VEC_PUSHBACK( pca_phi             );
      VEC_PUSHBACK( originalAlgo        );
      VEC_PUSHBACK( nHits               );
      VEC_PUSHBACK( nMissInnerHits      );
      VEC_PUSHBACK( nTrkLayers          );
      VEC_PUSHBACK( nMissTrkLayers      );
      VEC_PUSHBACK( nMissInnerTrkLayers );
      VEC_PUSHBACK( nMissOuterTrkLayers );
      VEC_PUSHBACK( nPxlLayers          );
      VEC_PUSHBACK( nMissPxlLayers      );
      VEC_PUSHBACK( nMissInnerPxlLayers );
      VEC_PUSHBACK( nMissOuterPxlLayers );
      VEC_PUSHBACK( ipXY                );
      // VEC_PUSHBACK( ipZ                 );
      VEC_PUSHBACK( ipXYSig             );
      VEC_PUSHBACK( dRToJetAxis         );
      VEC_PUSHBACK( distanceToJet       );
      VEC_PUSHBACK( vertexLxy           );
#undef VEC_PUSHBACK
      ipVector.push_back(fabs(dxy_ipv.second.value()));
      logIpSig.push_back(TMath::Log(fabs(dxy_ipv.second.significance())));


      misshits += itk->track().numberOfLostHits();
      tot_dsz += itk->track().dsz(theBeamSpot_->position());

      if (pca.isValid()) {
        r_pca.push_back(closestPoint.perp());
      }

      //           if (dxy_ipv.second.value() < ipCut) continue;
      if (dxy_ipv.second.significance() < minSigDispTrack) continue;

      // logSum += dxy_ipv.second.significance();

      Measurement1D ip2d = dxy_ipv.second;
      float r = 100*3.3*itk->track().pt()/3.8;
      float gLxy = ip2d.value()/sin(itk->track().phi()-jet.phi())*(1-2.5*fabs(ip2d.value())/r);

      sum_Lxy += gLxy;

      //           pTxIPxSig += itk->track().pt() * dxy_ipv.second.value() * dxy_ipv.second.significance() / jet->pt();
      pTxIPxSig += itk->track().pt() * dxy_ipv.second.significance() ;
      //           --promptTracks;
      ++nDispTracks;
      trackFrac += itk->track().pt();

      ipTracks.push_back(*itk);

    }
    trackFrac /= jet.pt();

    std::sort(logIpSig.begin(),logIpSig.end());
    std::sort(ipVector.begin(),   ipVector.end());
    std::sort(r_pca.begin(), r_pca.end());
    if (logIpSig.size() != 0) {
      medianLogIpSig = logIpSig[logIpSig.size()/2];
      if (logIpSig[logIpSig.size()/2] > logTagCut) {
        ++nTags;
      }
    }
  }
  // std::cout << "itrack: " << itrack << std::endl;
  // std::cout << "vec_ipXY.size(): " << vec_ipXY.size() << std::endl;

  // Calculate jet alpha_max
  double alpha_max = 0.;
  {
    reco::TrackRefVector trackRefs = jet.getTrackRefs();
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
      // std::cout << "New vertex\n";
      for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
        double trackWeight = ipv->trackWeight(*ijt);
        // std::cout << "Track weight: " << trackWeight << std::endl;
        if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
      } // End of track loop
      if (vertex_pt_sum > max_vertex_pt_sum) {
        max_vertex_pt_sum = vertex_pt_sum;
        ipv_chosen = ipv;
        // Calculate alpha
        alpha_max = vertex_pt_sum / jet_pt_sum;
      }
    } // End of vertex loop
  }
  // std::cout<< "Jet alpha_max: " << alpha_max << std::endl;

  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900111) continue;
        //                   std::cout << "genParticle pdgId, status = " << gp->status() << "\t" << gp->pdgId() << std::endl;
        //                   std::cout << "             mass = " << gp->mass() << std::endl;
        //                   std::cout << "               pT = " << gp->pt()   << std::endl;
        //                   std::cout << "      r, eta, phi = " << gp->vertex().r() << "\t" << gp->vertex().eta() << "\t" << gp->vertex().phi() << std::endl;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector_.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }

  bool matched = false;
  if (!isData_) {
    // Find out if there is a GenJet with deltaR < 0.2 relative to current jet
    for ( auto igj = genJets_->begin(); igj != genJets_->end(); ++igj) {
      TLorentzVector gjVector;
      gjVector.SetPtEtaPhiM(igj->pt(), igj->eta(), igj->phi(), 0.);
      if (gjVector.DeltaR(jetVector_) < 0.2) {
        matched = true;
        break;
      }
    }
  }

  int dtHits = 0;
  // Count DT hits with position vector that has deltaR < 0.5 relative to current jet
  for (std::vector<GlobalPoint>::iterator idt = dt_points_.begin(); idt != dt_points_.end(); ++idt) {
    TLorentzVector pointVector;
    pointVector.SetPtEtaPhiM(idt->perp(), idt->eta(), idt->phi(), 0.);
    if (pointVector.DeltaR(jetVector_) < 0.5) ++dtHits;
  }

  int cscHits = 0;
  // Count CSC hits with position vector that has deltaR < 0.5 relative to current jet
  for (std::vector<GlobalPoint>::iterator icp = csc_points_.begin(); icp != csc_points_.end(); ++icp) {
    TLorentzVector pointVector;
    pointVector.SetPtEtaPhiM(icp->perp(), icp->eta(), icp->phi(), 0.);
    if (pointVector.DeltaR(jetVector_) < 0.5) ++cscHits;
  }

  // Calculate number of vertices within jet cone, and median of displacement
  int matchedVertices = 0;
  float medianSVradius = 0.;
  {
    std::vector<float> radiusVector;
    // Loop over a given vertex vector/collection
    for (reco::VertexCollection::iterator ivtx = selectedSecondaryVertices_.begin(); ivtx != selectedSecondaryVertices_.end(); ++ivtx) {
      TLorentzVector vertexPosition;  vertexPosition.SetPtEtaPhiM(ivtx->position().r(),ivtx->position().eta(),ivtx->position().phi(),0.);
      if (vertexPosition.DeltaR(jetVector_) < 0.4) ++matchedVertices;
      if (ivtx->normalizedChi2() > 15.) continue;
      radiusVector.push_back(ivtx->position().r());
      TLorentzVector cand;
      // Build TransientTrack vector from refitted tracks
      std::vector<reco::TransientTrack> transRefitTracks;
      for (size_t itrack = 0; itrack < ivtx->refittedTracks().size(); itrack++) {
        transRefitTracks.push_back(transienttrackbuilderH_->build(ivtx->refittedTracks()[itrack]));
      }
    }
    std::sort(radiusVector.begin(), radiusVector.end());
    if (radiusVector.size() != 0) {
      medianSVradius = radiusVector.at(radiusVector.size()/2);
    }
  }

  // Fill vertex info from AVR vertices
  {
    // Macro expands to the following:
    // auto& <variable> = make_new_element (otree_.jet_<variable>)
#define GET_NEW_JET_VAR(a) auto& a = make_new_element (otree_.jet_##a)
     GET_NEW_JET_VAR( vertex_source ) ;
     GET_NEW_JET_VAR( vertex_x      ) ;
     GET_NEW_JET_VAR( vertex_y      ) ;
     GET_NEW_JET_VAR( vertex_z      ) ;
     GET_NEW_JET_VAR( vertex_xError ) ;
     GET_NEW_JET_VAR( vertex_yError ) ;
     GET_NEW_JET_VAR( vertex_zError ) ;
     GET_NEW_JET_VAR( vertex_deltaR ) ;
     GET_NEW_JET_VAR( vertex_Lxy    ) ;
     GET_NEW_JET_VAR( vertex_mass   ) ;
     GET_NEW_JET_VAR( vertex_chi2   ) ;
     GET_NEW_JET_VAR( vertex_ndof   ) ;
     GET_NEW_JET_VAR( vertex_pt2sum ) ;
#undef GET_NEW_JET_VAR
    for (TransientVertex vertex: avrVertices_) {
      int source = 1; // For AVR vertices
      auto vtx = reco::Vertex(vertex);
      TLorentzVector vertexVector;
      vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
      // Ignore vertices outside jet cone
      double deltaR = vertexVector.DeltaR(jetVector_) > 0.4;
      if (deltaR) continue;
      double Lxy = 0;
      float dx = primary_vertex.position().x() - vtx.position().x();
      float dy = primary_vertex.position().y() - vtx.position().y();
      Lxy = TMath::Sqrt( dx*dx + dy*dy );
      float mass = vtx.p4().mass();
      float pt2sum = calculatePt2Sum(vtx);
      vertex_source . push_back ( source       );
      vertex_x      . push_back ( vtx.x()      );
      vertex_y      . push_back ( vtx.y()      );
      vertex_z      . push_back ( vtx.z()      );
      vertex_xError . push_back ( vtx.xError() );
      vertex_yError . push_back ( vtx.yError() );
      vertex_zError . push_back ( vtx.zError() );
      vertex_deltaR . push_back ( deltaR       );
      vertex_Lxy    . push_back ( Lxy          );
      vertex_mass   . push_back ( mass         );
      vertex_chi2   . push_back ( vtx.chi2()   );
      vertex_ndof   . push_back ( vtx.ndof()   );
      vertex_pt2sum . push_back ( pt2sum       );
    }
  }

  otree_.jets_pt             .push_back( jet.pt()                          );
  otree_.jets_eta            .push_back( jet.eta()                         );
  otree_.jets_phi            .push_back( jet.phi()                         );
  otree_.jets_cef            .push_back( jet.chargedEmEnergyFraction()     );
  otree_.jets_nef            .push_back( jet.neutralEmEnergyFraction()     );
  otree_.jets_chf            .push_back( jet.chargedHadronEnergyFraction() );
  otree_.jets_nhf            .push_back( jet.neutralHadronEnergyFraction() );
  otree_.jets_phf            .push_back( jet.photonEnergyFraction()        );
  otree_.jets_nPromptTracks  .push_back( nPromptTracks                     );
  otree_.jets_nDispTracks    .push_back( nDispTracks                       );
  otree_.jets_nSV            .push_back( matchedVertices                   );
  otree_.jets_medianLogIpSig .push_back( medianLogIpSig                    );
  // otree_.jets_missHits       .push_back( misshits                           );
  // otree_.jets_muonHits       .push_back( dtHits+cscHits                     );
  otree_.jets_alphaMax       .push_back( alpha_max                         );
  otree_.jets_nDarkPions     .push_back( nDarkPions                        );
  otree_.jets_minDRDarkPion  .push_back( minDist                           );
  otree_.tracks_source              .push_back ( vec_source              ) ;
  otree_.tracks_pt                  .push_back ( vec_pt                  ) ;
  otree_.tracks_eta                 .push_back ( vec_eta                 ) ;
  otree_.tracks_phi                 .push_back ( vec_phi                 ) ;
  otree_.tracks_pca_r               .push_back ( vec_pca_r               ) ;
  otree_.tracks_pca_eta             .push_back ( vec_pca_eta             ) ;
  otree_.tracks_pca_phi             .push_back ( vec_pca_phi             ) ;
  otree_.tracks_algo                .push_back ( vec_algo                ) ;
  otree_.tracks_originalAlgo        .push_back ( vec_originalAlgo        ) ;
  otree_.tracks_nHits               .push_back ( vec_nHits               ) ;
  otree_.tracks_nMissInnerHits      .push_back ( vec_nMissInnerHits      ) ;
  otree_.tracks_nTrkLayers          .push_back ( vec_nTrkLayers          ) ;
  otree_.tracks_nMissInnerTrkLayers .push_back ( vec_nMissInnerTrkLayers ) ;
  otree_.tracks_nMissOuterTrkLayers .push_back ( vec_nMissOuterTrkLayers ) ;
  otree_.tracks_nMissTrkLayers      .push_back ( vec_nMissTrkLayers      ) ;
  otree_.tracks_nPxlLayers          .push_back ( vec_nPxlLayers          ) ;
  otree_.tracks_nMissInnerPxlLayers .push_back ( vec_nMissInnerPxlLayers ) ;
  otree_.tracks_nMissOuterPxlLayers .push_back ( vec_nMissOuterPxlLayers ) ;
  otree_.tracks_nMissPxlLayers      .push_back ( vec_nMissPxlLayers      ) ;
  otree_.tracks_ipXY           .push_back ( vec_ipXY           ) ;
  otree_.tracks_ipXYSig        .push_back ( vec_ipXYSig        ) ;
  // otree_.tracks_ipZ            .push_back ( vec_ipZ            ) ;
  otree_.tracks_dRToJetAxis    .push_back ( vec_dRToJetAxis    ) ;
  otree_.tracks_distanceToJet  .push_back ( vec_distanceToJet  ) ;


}

void
EmergingJetAnalyzer::fillVertexForSingleJet(const reco::VertexCollection&, int source) {
  while(0) {
    source++;
  }
}

reco::VertexCollection
EmergingJetAnalyzer::selectSecondaryVertices (edm::Handle<reco::VertexCollection> secondary_vertices) const {
  const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
  const int minDispSv = 0.1; // Minimum transverse displacement for a secondary vertex to be selected
  reco::VertexCollection selectedVertices;
  for (size_t ivx = 0; ivx < secondary_vertices->size(); ++ivx) {
    float dx = primary_vertex.position().x() - secondary_vertices->at(ivx).position().x();
    float dy = primary_vertex.position().y() - secondary_vertices->at(ivx).position().y();
    // If displacement from primary_vertex is greater than minDispSv
    if (TMath::Sqrt( dx*dx + dy*dy ) > minDispSv) {
        selectedVertices.push_back(secondary_vertices->at(ivx));
      }
  }
  return selectedVertices;
}

double
EmergingJetAnalyzer::calculatePt2Sum (const reco::Vertex& vertex) const {
  // Modified from reco::Vertex::p4()
  double sum = 0.;
  double pt2 = 0.;

  if(vertex.hasRefittedTracks()) {
    for(std::vector<reco::Track>::const_iterator iter = vertex.refittedTracks().begin();
        iter != vertex.refittedTracks().end(); ++iter) {
      pt2 = iter->pt();
      sum += pt2;
    }
  }
  else
    {
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = vertex.tracks_begin();
          iter != vertex.tracks_end(); iter++) {
        pt2 = (*iter)->pt();
        sum += pt2;
      }
    }
  return sum;
}

//define this as a plug-in
DEFINE_FWK_MODULE(EmergingJetAnalyzer);
