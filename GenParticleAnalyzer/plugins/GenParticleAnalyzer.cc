// -*- C++ -*-
//
// Package:    GenParticleAnalysis/GenParticleAnalyzer
// Class:      GenParticleAnalyzer
//
/**\class GenParticleAnalyzer GenParticleAnalyzer.cc GenParticleAnalysis/GenParticleAnalyzer/plugins/GenParticleAnalyzer.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Young Ho Shin
//         Created:  Thu, 04 Jun 2015 20:12:40 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"

//
// class declaration
//

class GenParticleAnalyzer : public edm::EDAnalyzer {
friend class EmJetAnalyzer;
public:
  explicit GenParticleAnalyzer(const edm::ParameterSet&);
  ~GenParticleAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  int getDecayLevel(const reco::Candidate*); // Function to get decay level of given particle
  static bool isDark(const reco::Candidate*); // Function to determine whether or not given particle is a Dark QCD particle
  static bool isDarkPion(const reco::Candidate*); // Function to determine whether or not given particle is a Dark pion
  static bool hasDarkDaughter(const reco::Candidate*); // Function to determine whether or not to given particle has Dark QCD daughters
  static bool hasDarkDescendent(const reco::Candidate*); // Function to determine whether or not to given particle has Dark QCD descendants
  static bool hasDarkMother(const reco::Candidate*); // Function to determine whether or not to given particle has Dark QCD mothers
  static bool hasDarkPionMother(const reco::Candidate*); // Function to determine whether or not to given particle has Dark pion mothers
  static void printParticle(const reco::Candidate*);
  static void printParticleMothers(const reco::Candidate*);
  struct ParticleToSort;
  typedef std::vector<ParticleToSort>::iterator PTSPtr; // typedef vector of pointers to ParticleToSort, for findParticleToSort(cand)
  bool isDark(const ParticleToSort p); //Overloaded function using sortedparticles
  PTSPtr findParticleToSort( const reco::Candidate* c);
  bool hasDarkDescendent(const ParticleToSort p); //Overloaded function using sortedparticles
  bool hasDarkDaughter(const ParticleToSort p); //Overloaded function using sortedparticles
  void printParticle(const ParticleToSort p); //Overloaded function using sortedparticles
  void assignMothersDaughters(PTSPtr p); // Function to assign mother and daughter indices to the ParticleToSort pointed to by PTSPtr
  std::string getParticleName(int index); // Function to get dark particle names

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------

  // inputs
  edm::EDGetTokenT<reco::GenParticleCollection> genParticleCollectionToken_;
  edm::Handle<reco::GenParticleCollection> genParticles;
  // outputs
  TFile* ofile_;
  TTree* tree_;
  std::vector<float> genParticlesPt_;
  std::vector<float> genParticlesEta_;
  std::vector<float> genParticlesPhi_;
  std::vector<float> genParticlesMass_;
  std::vector<int> genParticlesDecayLevel_;
  std::vector<int> genParticlesIndex_;
  std::vector<double> genParticlesVx_;
  std::vector<double> genParticlesVy_;
  std::vector<double> genParticlesVz_;
  std::vector<int> genParticleIsDark_; // 0 and 1 for false and true
  std::vector<int> genParticleIsSMWithDarkMother_; // -1 and 1 for false and true
  std::vector<int> genParticleIsDarkWithSMDaughter_; // -1 and 1 for false and true
  std::vector<float> genParticleLxy_;
  std::vector<float> genParticleDecayTime_;
  std::vector<int> genParticlePdgId_;
  // Struct containing index of particle in collection, decay levels, priorities, etc.
  struct ParticleToSort {
    const reco::Candidate* cand; //ptr to the genParticle
    int index; // particle index in GenParticleCollection
    int decayLevel;
    int verbosityToPrint; // Print only if verbosity >= verbosityToPrint
    std::vector<int> iMothers; // mother indices
    std::vector<int> iDaughters; // daughter indices
  };
  std::vector<ParticleToSort> sortedParticles;
};

//
// constants, enums and typedefs
//
const int verbosity = 10;

//
// static data member definitions
//

//
// constructors and destructor
//
GenParticleAnalyzer::GenParticleAnalyzer(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  ofile_ = new TFile("file.root","RECREATE","GenParticleAnalyzer output file");
  tree_ = new TTree("tree", "GenParticleAnalyzer output tree");
  tree_->Branch("pt"                 , &genParticlesPt_                ) ;
  tree_->Branch("eta"                , &genParticlesEta_               ) ;
  tree_->Branch("phi"                , &genParticlesPhi_               ) ;
  tree_->Branch("mass"               , &genParticlesMass_              ) ;
  tree_->Branch("decay_level"        , &genParticlesDecayLevel_        ) ;
  tree_->Branch("index"              , &genParticlesIndex_             ) ;
  tree_->Branch("vertex_x"           , &genParticlesVx_                ) ;
  tree_->Branch("vertex_y"           , &genParticlesVy_                ) ;
  tree_->Branch("vertex_z"           , &genParticlesVz_                ) ;
  tree_->Branch("isDark"             , &genParticleIsDark_             ) ;
  tree_->Branch("isSMWithDarkMother" , &genParticleIsSMWithDarkMother_ ) ;
  tree_->Branch("isDarkWithSMDaughter" , &genParticleIsDarkWithSMDaughter_ ) ;
  tree_->Branch("Lxy"                , &genParticleLxy_                ) ;
  tree_->Branch("decayTime"          , &genParticleDecayTime_          ) ;
  tree_->Branch("pdgId"		     , &genParticlePdgId_	       ) ;
}


GenParticleAnalyzer::~GenParticleAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  ofile_->Write();
  delete ofile_;

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenParticleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  iEvent.getByLabel("genParticles", genParticles);

  genParticlesPt_                . clear();
  genParticlesEta_               . clear();
  genParticlesPhi_               . clear();
  genParticlesMass_              . clear();
  genParticlesDecayLevel_        . clear();
  genParticlesIndex_             . clear();
  genParticlesVx_                . clear();
  genParticlesVy_                . clear();
  genParticlesVz_                . clear();
  genParticleIsDark_             . clear();
  genParticleIsSMWithDarkMother_ . clear();
  genParticleIsDarkWithSMDaughter_ . clear();
  genParticleLxy_                . clear();
  genParticleDecayTime_          . clear();
  genParticlePdgId_		 . clear();
  sortedParticles         . clear();

  int index = 0;
  for (auto it = genParticles->begin(); it != genParticles->end(); it++) {
    //std::cout << "Running over particle " << index << std::endl;
    auto& candidate = *it;
    //if (candidate.numberOfMothers()>1) std::cout <<"Particle with more than one mother " << index << " " << candidate.pdgId() << "\n";
    const reco::Candidate* cand = &(*it);
    int decayLevel = getDecayLevel(cand);
    // std::cout << index << ": particle pdg " << candidate.pdgId()<< " decay level "<< decayLevel <<std::endl;
    int verbosityToPrint = 1000, labelIsDark = 0, labelIsSMWithDarkMother = -1, labelIsDarkWithSMDaughter = -1;
    if (hasDarkDescendent(cand)) verbosityToPrint = 2;
    if (hasDarkDaughter(cand))   verbosityToPrint = 1;
    if (isDark(cand)) {          verbosityToPrint = 0; labelIsDark = 1;// Always print dark particles; change labelIsDark to 1 (true)
    }
    //    if ( (isDark(cand) == false) && hasDarkMother(cand) ) { labelIsSMWithDarkMother = 1; //mark candidate as a SM daughter from a dark QCD mother
    //    }
    sortedParticles.push_back({cand, index, decayLevel, verbosityToPrint});
    index ++;
    TLorentzVector momentum;
    momentum.SetPtEtaPhiM(candidate.pt(), candidate.eta(), candidate.phi(), candidate.mass());
    double Lxy = -1;
    if ( candidate.numberOfDaughters()>0 ) {
      const reco::Candidate* dau = candidate.daughter(0);
      std::cout << "dau: " << dau << std::endl;
      if (dau) {
        std::cout << "Valid dau " << std::endl;
        Lxy = TMath::Sqrt( dau->vx()*dau->vx() + dau->vy()*dau->vy() );
      }
    }
    double decayTime = Lxy / momentum.Beta() / momentum.Gamma();

    genParticlesPt_                . push_back( candidate . pt()   );
    genParticlesEta_               . push_back( candidate . eta()  );
    genParticlesPhi_               . push_back( candidate . phi()  );
    genParticlesMass_              . push_back( candidate . mass() );
    genParticlesDecayLevel_        . push_back( decayLevel         ); //write decayLevel to branch
    genParticlesIndex_             . push_back( index              ); //write genParticle index to branch
    genParticlesVx_                . push_back( candidate . vx()   ); //write vertex x coordinate to branch
    genParticlesVy_                . push_back( candidate . vy()   ); //write vertex y coordinate to branch
    genParticlesVz_                . push_back( candidate . vz()   ); //write vertex z coordinate to branch
    genParticleIsDark_             . push_back( labelIsDark	    ); //write isDark() boolean value to branch
    genParticleIsSMWithDarkMother_ . push_back( labelIsSMWithDarkMother );
    genParticleIsDarkWithSMDaughter_ . push_back( labelIsDarkWithSMDaughter );
    genParticleLxy_                . push_back( Lxy );
    genParticleDecayTime_          . push_back( decayTime );
    genParticlePdgId_		   . push_back( candidate . pdgId());// write pdgId to branch
  }
  std::cout << "Number of particles: " << genParticles->size() << std::endl;

  //sorting the vector according to decayLevels and priority level
  std::sort( sortedParticles.begin(), sortedParticles.end(),
             [&](const ParticleToSort & lhs, const ParticleToSort & rhs){
               if (lhs.decayLevel == rhs.decayLevel) return ( abs(genParticles->at(lhs.index).pdgId()) > abs(genParticles->at(rhs.index).pdgId()) );
               else return (lhs.decayLevel < rhs.decayLevel) ;
             });
  //print out pdgId by decay levels
  int level = -1;
  for (auto i = sortedParticles.begin(); i != sortedParticles.end(); i++) {
    //assign labelIsSMWithDarkMother = 1 to the first SM daughter of dark candidates
    if ( (isDark(*i) == true) && (hasDarkDaughter(*i) == false) && (i->cand->numberOfDaughters()) ) {
      genParticleIsSMWithDarkMother_[ findParticleToSort( i->cand->daughter(0) ) -> index ] = 1;
      genParticleIsDarkWithSMDaughter_[ findParticleToSort( i->cand ) -> index ] = 1;
    }


    if (((i->decayLevel) > level)) {
      level = (i->decayLevel);//update level number
      std::cout << std::endl <<"Decay level: " << level << std::endl;
    }
    if (((i->decayLevel) == level)) {
      // std::cout << ".index=" << i->index << ".";
      if ( verbosity >= i->verbosityToPrint ) {
        // std::cout<< ".v"<<i->verbosityToPrint << "v";
        std::cout<<"     || (" << i->index <<")"<<getParticleName(i->index) << " -> "<< " ";
        assignMothersDaughters(i);
        for (int x : std::vector<int> (i->iDaughters)) std::cout <<"("<< x <<")"<< getParticleName(x)<< " ";
      }
    }
    else {
      std::cout<<"particle at decay level " << (i->decayLevel) <<" is not correctly sorted.\n";
    }
  }
  std::cout << std::endl;
  tree_->Fill();


#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
GenParticleAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
GenParticleAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
/*
  void
  GenParticleAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when ending the processing of a run  ------------
/*
  void
  GenParticleAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
  void
  GenParticleAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
  void
  GenParticleAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenParticleAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// Function to get decay level of a given particle
int
GenParticleAnalyzer::getDecayLevel(const reco::Candidate* cand) {
  if ( cand->numberOfMothers()==0 ) return 0; //decay level 0
  //  if ( cand->numberOfMothers()==1 && cand->mother()->numberOfDaughters()==1 && cand->mother()->pdgId()==cand->pdgId() ) return getDecayLevel( cand->mother() );
  else {
    int minDecayLevel = 999999;
    // For each mother particle, call decayLevel(mother) and find minimum.
    for (unsigned i = 0; i < cand->numberOfMothers(); i++) {
      int level = getDecayLevel( cand->mother(i) );
      if (level<minDecayLevel) minDecayLevel = level;
    }
    return minDecayLevel + 1;
  }
}

// function to find the ParticleToSort in sortedParticles
GenParticleAnalyzer::PTSPtr
GenParticleAnalyzer::findParticleToSort( const reco::Candidate* c){
  for (PTSPtr sp = sortedParticles.begin(); sp != sortedParticles.end(); ++sp){
    if (c == sp->cand ) return sp;
  }
  return PTSPtr(NULL);
}

// function to assign mother and daughter indices to a ParticleToSort, using its pointer PTSPtr
void
GenParticleAnalyzer::assignMothersDaughters(PTSPtr p){
  for ( unsigned int i = 0; i != p->cand->numberOfDaughters(); ++i){
    auto dauPTS = findParticleToSort(p->cand->daughter(i));
    dauPTS->iMothers.push_back(p->index);
    p->iDaughters.push_back(dauPTS->index);
  }
}

// function to get the dark candidates with SM daughters, for finding the secondary vertices of emerging jets

//function to get name of a dark particle
std::string GenParticleAnalyzer::getParticleName(int index){
  int id = genParticles->at(index).pdgId();//get pdgId
  if (id == 4900001 ) return "d_X";
  if (id == 4900021 ) return "d_G";
  if (id == 4900101 ) return "d_Q";
  if (id == 4900111 ) return "d_Pi";
  if (id == 4900113 ) return "d_Rho";
  if (id == -4900001 ) return "d_X-";
  if (id == -4900021 ) return "d_G-";
  if (id == -4900101 ) return "d_Q-";
  if (id == -4900111 ) return "d_Pi-";
  if (id == -4900113 ) return "d_Rho-";
  else return std::to_string(id);
}
bool
GenParticleAnalyzer::isDark(const reco::Candidate* cand) {
  if ( abs(cand->pdgId()) == 4900001 ) return true; // Bi-fundamental mediator
  if ( abs(cand->pdgId()) == 4900021 ) return true; // Dark gluon
  if ( abs(cand->pdgId()) == 4900101 ) return true; // Dark quark
  if ( abs(cand->pdgId()) == 4900111 ) return true; // Dark pion
  if ( abs(cand->pdgId()) == 4900113 ) return true; // Dark rho
  return false;
}

bool
GenParticleAnalyzer::isDarkPion(const reco::Candidate* cand) {
  if ( abs(cand->pdgId()) == 4900111 ) return true; // Dark pion
  return false;
}

bool
GenParticleAnalyzer::hasDarkDaughter(const reco::Candidate* cand) {
  for (unsigned i = 0; i < cand->numberOfDaughters(); i++) {
    if ( isDark(cand->daughter(i)) ) return true;
  }
  return false;
}

bool
GenParticleAnalyzer::hasDarkMother(const reco::Candidate* cand) {
  for (unsigned i = 0; i < cand->numberOfMothers(); i++) {
    if ( isDark(cand->mother(i)) ) return true;
  }
  return false;
}

bool
GenParticleAnalyzer::hasDarkPionMother(const reco::Candidate* cand) {
  for (unsigned i = 0; i < cand->numberOfMothers(); i++) {
    if ( isDarkPion(cand->mother(i)) ) return true;
  }
  return false;
}

bool
GenParticleAnalyzer::hasDarkDescendent(const reco::Candidate* cand) {
  // Function to determine whether or not to given particle has Dark QCD descendants
  for (unsigned i = 0; i < cand->numberOfDaughters(); i++) {
    if ( hasDarkDescendent(cand->daughter(i)) ) return true;
  }
  return false;
}

void
GenParticleAnalyzer::printParticle(const reco::Candidate* cand) {
  std::cout << cand->pdgId() <<" ";
}

void
GenParticleAnalyzer::printParticleMothers(const reco::Candidate* cand) {
  std::cout << "Mothers pdgId: ";
  for (unsigned i = 0; i < cand->numberOfMothers(); i++) {
    std::cout << cand->mother(i)->pdgId() << ", ";
  }
  std::cout << std::endl;
}
////
//alternative functions to above using ParticleToSort struct
////
void
GenParticleAnalyzer::printParticle(const ParticleToSort p) {
  std::cout << genParticles->at(p.index).pdgId() <<" ";
}

bool
GenParticleAnalyzer::isDark(const ParticleToSort p) {
  if ( genParticles->at(p.index).pdgId() == 4900001 ) return true; // Bi-fundamental mediator
  if ( genParticles->at(p.index).pdgId() == 4900021 ) return true; // Dark gluon
  if ( genParticles->at(p.index).pdgId() == 4900101 ) return true; // Dark quark
  if ( genParticles->at(p.index).pdgId() == 4900111 ) return true; // Dark pion
  if ( genParticles->at(p.index).pdgId() == 4900113 ) return true; // Dark rho
  return false;
}

bool
GenParticleAnalyzer::hasDarkDaughter(const ParticleToSort p) {
  for (unsigned i = 0; i < genParticles->at(p.index).numberOfDaughters(); i++) {
    if ( isDark( genParticles->at(p.index).daughter(i) ) ) return true;
  }
  return false;
}

bool
GenParticleAnalyzer::hasDarkDescendent(const ParticleToSort p) {
  // Function to determine whether or not to given particle has Dark QCD descendants
  for (unsigned i = 0; i < genParticles->at(p.index).numberOfDaughters(); i++) {
    if ( (genParticles->at(p.index).daughter(i)) ) return true;
  }
  return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenParticleAnalyzer);
