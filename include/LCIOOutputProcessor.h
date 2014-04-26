#ifndef LCIOOutputProcessor_h
#define LCIOOutputProcessor_h 
 
#include "Processor.h"
#include "lcio.h"
#include "IO/LCWriter.h"


using namespace lcio ;

namespace marlin{

/** Default output processor. If active every event is writen to the 
 *  specified LCIO file.  
 *  Make sure that the processor is the last one in your list
 *  of active processors. You can optionally drop some collections from the
 *  event before it gets written to the file, e.g. you can drop 
 *  all collections of types SimCalorimeterHit and SimTrackerHit. It is the users
 *  responsibility to check whether the droped objects are still referenced by other 
 *  objects (e.g. LCRelations) and drop those collections as well - if needed.
 *  Otherwise NULL pointers are returned by the corresponding access methods.
 * 
 *  <h4>Input - Prerequisites</h4>
 *  Uses the whole event, i.e. all collections. 
 *
 *  <h4>Output</h4> 
 *  none
 * 
 * @param LCIOOutputFile  name of outputfile incl. path
 * @param LCIOWriteMode   WRITE_NEW, WRITE_APPEND  [optional]
 * @param DropCollectionNames   name of collections to be droped  [optional]
 * @param DropCollectionTypes   type of collections to be droped  [optional]
 * 
 * 
 * @param DropCollectionNames  drops the named collections from the event
 * @param DropCollectionTypes  drops all collections of the given type from the event
 * @param LCIOOutputFile       name of output file
 * @param LCIOWriteMode        write mode for output file:  WRITE_APPEND or WRITE_NEW
 *
 * @author F. Gaede, DESY
 * @version $Id: LCIOOutputProcessor.h,v 1.6 2006/03/06 15:25:04 gaede Exp $ 
 */
class LCIOOutputProcessor : public Processor {
  
	public:  

  		virtual Processor*  newProcessor() { return new LCIOOutputProcessor ; }
    
  		LCIOOutputProcessor() ;

  		//Open the LCIO outputfile.
  		virtual void init() ;

  		// Write every run header.
  		virtual void processRunHeader( LCRunHeader* run) ;

  		// Write every event.
  		virtual void processEvent( LCEvent * evt ) ; 

  		// Close outputfile.

  		virtual void end() ;

  		// Drops the collections specified in the steering file parameters DropCollectionNames and DropCollectionTypes. 
  		void dropCollections( LCEvent * evt ) ;

	protected:

  		std::string _lcioOutputFile ;
  		std::string _lcioWriteMode ; 

  		StringVec _dropCollectionNames ; 
  		StringVec _dropCollectionTypes ; 

  		int _splitFileSizekB ;

  		LCWriter* _lcWrt ;
  		int _nRun ;
  		int _nEvt ;
} ;

} // end namespace marlin 
#endif
