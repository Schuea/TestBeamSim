#ifndef COLLECTION_PROCESSOR_H__
#define COLLECTION_PROCESSOR_H__

#include "marlin/Processor.h"
#include <EVENT/MCParticle.h>
#include <EVENT/SimCalorimeterHit.h>
#include <EVENT/LCGenericObject.h>
#include <EVENT/LCEvent.h>
#include <EVENT/LCCollection.h>

#include <IMPL/LCCollectionVec.h>

#ifdef USE_MARLIN
// streamlog include
#include "streamlog/streamlog.h"
#endif
#include "TTree.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#define _USE_MATH_DEFINES

using namespace std;

#define PDGID_GAMMA 22
#define PDGID_POSITRON -11
#define PDGID_ELECTRON 11


template<typename T>
struct TypeNameCollection;

#define REGISTER_COLLECTION(X) template <> struct TypeNameCollection<X> \
    { static const char* TypeName; } ; const char* TypeNameCollection<X>::TypeName = #X

REGISTER_COLLECTION(EVENT::MCParticle);
REGISTER_COLLECTION(EVENT::SimCalorimeterHit);
REGISTER_COLLECTION(EVENT::LCGenericObject);


class Collection_Processor_Interface{

	public:

	Collection_Processor_Interface (const char* name,
		const char* description,const char* defaultName):name_(name),description_(description),defaultName_(defaultName){}
	
	virtual ~Collection_Processor_Interface (){std::cout<<"~Collection_Processor_Interface"<<std::endl;}	

	virtual void processCurrentEvent_InputCollection (lcio::LCEvent * evt)=0;
	
	
	virtual std::string& getTypeName()=0;
	std::string& getName(){return name_;}
	std::string& getDescription(){return description_;}
	std::string& getCol(){return col_;}
	std::string& getDefaultName(){return defaultName_;}

	private:

	std::string name_,description_,col_,defaultName_;

}; 


template <typename T>
class Collection_Processor_Template : public Collection_Processor_Interface{

	public:

	Collection_Processor_Template(const char* name,
				const char* description,
				const char* defaultName):Collection_Processor_Interface(name,description,defaultName){
		Typename_=std::string(TypeNameCollection<T>::TypeName);
	}

	virtual ~Collection_Processor_Template(){
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
		std::cout<<"~Collection_Processor_Template()"<<Typename_<<std::endl;
	}

	virtual void processCurrentEvent_InputCollection( lcio::LCEvent * evt){
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
		EVENT::LCCollection* col=evt->getCollection(getCol());
		
		tree_reset();
			int nMCP = col->getNumberOfElements()  ;
		//std::cout<<nMCP<<std::endl;
			for(int i=0; i< nMCP ; i++){
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
				T* p = dynamic_cast<T*>( col->getElementAt(i));		
				fillInHist(p,evt);
				tree_fill();
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
			}
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
	}

	virtual std::string& getTypeName(){
		return  Typename_;
	}

	virtual void fillInHist(T *p, lcio::LCEvent * evt)=0;
	virtual void tree_fill()=0;
	virtual void tree_reset()=0;

	std::string Typename_;
};


class InputCollectionProcessor_MCParticle_collection: public Collection_Processor_Template<EVENT::MCParticle>{

	public:
	
	InputCollectionProcessor_MCParticle_collection(TTree* tree,const char* defaultName):Collection_Processor_Template("InputCollectionName",
				 "Collection name for MC Particles",defaultName),tree_(tree){		
		z_axis_ = new float[3];
		z_axis_[0] = 0.0;
		z_axis_[1] = 0.0;
		z_axis_[2] = 1.0;
		registerTTree();
		tree_reset();
	}
	~InputCollectionProcessor_MCParticle_collection(){
		std::cout<<"~InputCollectionProcessor_MCParticle_collection"<<std::endl;
	}

	float scalar_product(float a[3], float b[3]){
		float result = 0;
		for (int d=0; d<3; d++) {
			result += a[d]*b[d];
		}
		return result;
	}

	float norm(float a[3]){
		float result = 0;
		float tmp=0;
		for (int d=0; d<3; d++) {
			tmp += a[d]*a[d];
		}
		result = sqrt(tmp);
		return result;
	}
/*
	float GetFinalParticle_Course(MCParticle*p){
		//if (p->getDaughters().empty()){
		//if (p->getParents().empty()){
			float result[3]={};
			for (int d=0; d < 3; d++){
				result[d] = p->getEndpoint()[d] - p->getVertex()[d];
			}
			return result;	
		}
		else{
			for (int n=0; n < p->getDaughters().size(); n++){
				GetFinalParticle_Course(p->getDaughters().at(n));
			}
		}
	}
*/	
	virtual void fillInHist(EVENT::MCParticle *p, lcio::LCEvent * evt){
streamlog_out(DEBUG0)<< "Line " << __LINE__  << " File " << __FILE__ << endl;
		event_id_= evt->getEventNumber();
		particle_id_ = p->getPDG();
		
		numberOfParents_ = p->getParents().size();
		numberOfDaughters_ = p->getDaughters().size();
		streamlog_out(DEBUG0) << "event_id_: " << event_id_ << endl;	
		streamlog_out(DEBUG0) << "numberOfParents_: " << p->getParents().size() << endl;
		if (numberOfParents_==1){
			parent_id_ = p->getParents().at(0)->getPDG();
			parent2_id_=0;
			streamlog_out(DEBUG0) << "p->getParents().at(0) : " << p->getParents().at(0) << endl;
			streamlog_out(DEBUG0) << "parent_id_: " << parent_id_ << endl;
			streamlog_out(DEBUG0) << "parent2_id_: " << parent2_id_ << endl;
		}
		else if (numberOfParents_>1){
			parent_id_ = p->getParents().at(0)->getPDG();
		 	parent2_id_ = p->getParents().at(1)->getPDG();
			streamlog_out(DEBUG0) << "p->getParents().at(0) : " << p->getParents().at(0) << endl;
			streamlog_out(DEBUG0) << "p->getParents().at(1) : " << p->getParents().at(1) << endl;
			streamlog_out(DEBUG0) << "parent_id_: " << parent_id_ << endl;
			streamlog_out(DEBUG0) << "parent2_id_: " << parent2_id_ << endl;
		}	
		else{
			parent_id_=0;
			parent2_id_=0;
		}
		streamlog_out(DEBUG0) << "parent_id_: " << parent_id_ << endl;
		streamlog_out(DEBUG0) << "parent2_id_: " << parent2_id_ << endl;
	
		createdInSimulation_status_ = p->isCreatedInSimulation(); 
		decayedInTracker_status_ = p->isDecayedInTracker(); 
		hasLeftDetector_status_ = p->hasLeftDetector();
		createdInContinuousProcess_status_ = p->vertexIsNotEndpointOfParent();
		stopped_status_ = p->isStopped();
		
		charge_ = p->getCharge();
		energy_ = p->getEnergy();
		momentum_ = sqrt( (p->getMomentum()[0])*(p->getMomentum()[0]) + (p->getMomentum()[1])*(p->getMomentum()[1]) + (p->getMomentum()[2])*(p->getMomentum()[2]) );
				
		reflectionx_ = p->getEndpoint()[0];
		reflectiony_ = p->getEndpoint()[1];
		reflectionz_ = p->getEndpoint()[2];

		vertexx_ = p->getVertex()[0];
		vertexy_ = p->getVertex()[1];
		vertexz_ = p->getVertex()[2];

		if (numberOfParents_==0){ //First particle in simulation list: initial particle from generator
			for (int d=0; d < 3; d++){
				if (numberOfDaughters_==0){
					initialparticle_course_[d] = p->getEndpoint()[d] - p->getVertex()[d];
					streamlog_out(DEBUG0) << "initialparticle_course_[" <<d<< "] = " << initialparticle_course_[d] << endl;
					angle_initialfinal_=0;
				
					theta_= 360.0/(2.0*M_PI) * acos(scalar_product(z_axis_,initialparticle_course_)/(norm(z_axis_)*norm(initialparticle_course_)));
					eta_= -log(tan((theta_*2*M_PI/360)/2));
	
					streamlog_out(DEBUG0) << "theta_ = " << theta_ << endl;
					streamlog_out(DEBUG0) << "eta_ = " << eta_ << endl;
				}
				else{
					initialparticle_course_[d] = p->getDaughters().at(0)->getVertex()[d] - p->getVertex()[d];
					streamlog_out(DEBUG0) << "initialparticle_course_[" <<d<< "] = " << initialparticle_course_[d] << endl;
					
					theta_= 360.0/(2.0*M_PI) * acos(scalar_product(z_axis_,initialparticle_course_)/(norm(z_axis_)*norm(initialparticle_course_)));
					eta_= -log(tan((theta_*2*M_PI/360)/2));
	
					streamlog_out(DEBUG0) << "theta_ = " << theta_ << endl;
					streamlog_out(DEBUG0) << "eta_ = " << eta_ << endl;
				} 
			}
		}
		else{
			//finalparticle_course_ = GetFinalParticle_Course(p);
			if (p->hasLeftDetector()==true){
				for (int d=0; d < 3; d++){
					finalparticle_course_[d] = p->getEndpoint()[d] - p->getVertex()[d];
					streamlog_out(DEBUG0) << "initialparticle_course_[" <<d<< "] = " << initialparticle_course_[d] << endl;
					streamlog_out(DEBUG0) << "finalparticle_course_[" <<d<< "] = " << finalparticle_course_[d] << endl;
				}
				streamlog_out(DEBUG0) << "scalar_product(initialparticle_course_,finalparticle_course_) = " << scalar_product(initialparticle_course_,finalparticle_course_) << endl;
				streamlog_out(DEBUG0) << "norm(initialparticle_course_) = " << norm(initialparticle_course_)<< endl;
				streamlog_out(DEBUG0) << "norm(finalparticle_course_) = " << norm(finalparticle_course_)<< endl;
				angle_initialfinal_=360/(2*M_PI) * acos(scalar_product(initialparticle_course_,finalparticle_course_)/(norm(initialparticle_course_)*norm(finalparticle_course_)));
				streamlog_out(DEBUG0) << "angle_initialfinal_ = " << angle_initialfinal_ << endl;
			
				theta_= 360.0/(2.0*M_PI) * acos(scalar_product(z_axis_,finalparticle_course_)/(norm(z_axis_)*norm(finalparticle_course_)));
				eta_= -log(tan((theta_*2*M_PI/360)/2));
	
				streamlog_out(DEBUG0) << "theta_ = " << theta_ << endl;
				streamlog_out(DEBUG0) << "eta_ = " << eta_ << endl;
			}
			else{
				angle_initialfinal_=0;
			}
		}
		
	}

	virtual void tree_reset(){
		event_id_=0;
		particle_id_=0;
		parent_id_=0;
		parent2_id_=0;
		numberOfParents_=0;		
		numberOfDaughters_=0;		

		createdInSimulation_status_=-1;
		decayedInTracker_status_=-1;
		hasLeftDetector_status_=-1;
 		createdInContinuousProcess_status_=-1;
		stopped_status_=-1;

		charge_=0;
		energy_=0;
		momentum_=0;
        	reflectionx_=0;
        	reflectiony_=0;
        	reflectionz_=0;
	       	vertexx_=0;
        	vertexy_=0;
        	vertexz_=0;
		angle_initialfinal_=0;
		theta_=0;
		eta_=0;

		for(int i = 0; i < 3; ++i){
			initialparticle_course_[i]=0;
			finalparticle_course_[i]=0;
		}	

	}

	virtual void tree_fill(){
		tree_->Fill();
	}

 	void registerTTree(){

		tree_->Branch("Event_ID",&event_id_,"Event_ID/I");
		tree_->Branch("Particle_ID",&particle_id_,"Particle_ID/I");
		tree_->Branch("Parent_ID",&parent_id_,"Parent_ID/I");
		tree_->Branch("Parent2_ID",&parent2_id_,"Parent2_ID/I");
		tree_->Branch("NumberOfParents",&numberOfParents_,"NumberOfParents/I");
		tree_->Branch("NumberOfDaughters",&numberOfDaughters_,"NumberOfDaughters/I");
		
		tree_->Branch("CreatedInSimulation_Status",&createdInSimulation_status_,"CreatedInSimulation_Status/O");	
		tree_->Branch("DecayedInTracker_Status",&decayedInTracker_status_,"DecayedInTracker_Status/O");	
		tree_->Branch("hasLeftDetector_Status",&hasLeftDetector_status_,"hasLeftDetector_Status/O");	
		tree_->Branch("createdInContinuousProcess_Status",&createdInContinuousProcess_status_,"createdInContinuousProcess_Status/O");	
		tree_->Branch("Stopped_Status",&stopped_status_,"Stopped_Status/O");	

		tree_->Branch("Charge",&charge_,"Charge/F");
		tree_->Branch("Energy",&energy_,"Energy/F");
		tree_->Branch("Momentum",&momentum_,"Momentum/F");
		tree_->Branch("Reflectionx",&reflectionx_,"Reflectionx/F");
		tree_->Branch("Reflectiony",&reflectiony_,"Reflectiony/F");
		tree_->Branch("Reflectionz",&reflectionz_,"Reflectionz/F");
		tree_->Branch("Vertexx",&vertexx_,"Vertexx/F");
		tree_->Branch("Vertexy",&vertexy_,"Vertexy/F");
		tree_->Branch("Vertexz",&vertexz_,"Vertexz/F");

		tree_->Branch("Angle_initialfinal",&angle_initialfinal_,"Angle_initialfinal/F");	
		tree_->Branch("Theta",&theta_,"Theta/F");	
		tree_->Branch("Eta",&eta_,"Eta/F");	

cout << "Line " << __LINE__  << " File " << __FILE__ << endl;
	}


	private:

	TTree* tree_;

	int event_id_;
	int particle_id_;
	int parent_id_;
	int parent2_id_;
	int numberOfParents_;
	int numberOfDaughters_;
	
	bool createdInSimulation_status_;
	bool decayedInTracker_status_;
	bool hasLeftDetector_status_;
	bool createdInContinuousProcess_status_;
	bool stopped_status_;
        
	float charge_;
	float energy_;
	float momentum_;
	float reflectionx_;
	float reflectiony_;
	float reflectionz_;
	float vertexx_;
	float vertexy_;
	float vertexz_;

	float angle_initialfinal_;
	float theta_;
	float eta_;
 
	float *z_axis_;
	float initialparticle_course_[3];
	float finalparticle_course_[3];
	
	
};


class InputCollectionProcessor_SimCalorimeterHit_collection: public Collection_Processor_Template<EVENT::SimCalorimeterHit>{

	public:
	
	InputCollectionProcessor_SimCalorimeterHit_collection(TTree* tree,const char* defaultName):Collection_Processor_Template	("InputCollectionName2",
				 "Collection name for SimCalorimeterHits",defaultName),tree_(tree){		
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
		registerTTree();
		tree_reset();
streamlog_out(DEBUG0) << "Line " << __LINE__  << " File " << __FILE__ << endl;
	}

//____________EDIT:

	virtual void fillInHist(EVENT::SimCalorimeterHit *p, lcio::LCEvent * evt){
/*
		event_id_= evt->getEventNumber();
		particle_id_ = p->getPDG();
		
		energy_ = p->getEnergy();
				
		reflectionx_ = p->getEndpoint()[0];
		reflectiony_ = p->getEndpoint()[1];
		reflectionz_ = p->getEndpoint()[2];
*/
	}

	virtual void tree_reset(){
/*
		event_id_=0;
		particle_id_=0;

		energy_=0;
        	reflectionx_=0;
        	reflectiony_=0;
        	reflectionz_=0;
*/
	}

	virtual void tree_fill(){
		tree_->Fill();
	}

 	void registerTTree(){
/*
		tree_->Branch("Event_ID",&event_id_,"Event_ID/I");
		tree_->Branch("Particle_ID",&particle_id_,"Particle_ID/I");
	
		tree_->Branch("Energy",&energy_,"Energy/F");
		tree_->Branch("Reflectionx",&reflectionx_,"Reflectionx/F");
		tree_->Branch("Reflectiony",&reflectiony_,"Reflectiony/F");
		tree_->Branch("Reflectionz",&reflectionz_,"Reflectionz/F");
*/
	}

	private:

	TTree* tree_;
/*
	int event_id_;
	int particle_id_;

        float energy_;
	float reflectionx_;
	float reflectiony_;
	float reflectionz_;
*/
//_____________________________

};

class InputCollectionProcessor_LCGenericObject_collection: public Collection_Processor_Template<EVENT::LCGenericObject>{

	public:
	
	InputCollectionProcessor_LCGenericObject_collection(TTree* tree,const char* defaultName):Collection_Processor_Template("InputCollectionName3",
				 "Collection name for LCGenericObject",defaultName),tree_(tree){		
		registerTTree();
		tree_reset();
	}

//____________EDIT:

	virtual void fillInHist(EVENT::LCGenericObject *p, lcio::LCEvent * evt){
/*
		event_id_= evt->getEventNumber();
		particle_id_ = p->getPDG();
		
		energy_ = p->getEnergy();
				
		reflectionx_ = p->getEndpoint()[0];
		reflectiony_ = p->getEndpoint()[1];
		reflectionz_ = p->getEndpoint()[2];
*/
	}

	virtual void tree_reset(){
/*
		event_id_=0;
		particle_id_=0;

		energy_=0;
        	reflectionx_=0;
        	reflectiony_=0;
        	reflectionz_=0;
*/
	}

	virtual void tree_fill(){
		tree_->Fill();
	}

 	void registerTTree(){
/*
		tree_->Branch("Event_ID",&event_id_,"Event_ID/I");
		tree_->Branch("Particle_ID",&particle_id_,"Particle_ID/I");
	
		tree_->Branch("Energy",&energy_,"Energy/F");
		tree_->Branch("Reflectionx",&reflectionx_,"Reflectionx/F");
		tree_->Branch("Reflectiony",&reflectiony_,"Reflectiony/F");
		tree_->Branch("Reflectionz",&reflectionz_,"Reflectionz/F");
*/
	}

	private:

	TTree* tree_;
/*
	int event_id_;
	int particle_id_;

        float energy_;
	float reflectionx_;
	float reflectiony_;
	float reflectionz_;
*/
//_____________________________

};

#endif
