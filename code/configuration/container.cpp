// *****************************************************************************************
//
// File description:
//
// Author:	Joao Costa
// Purpose:	Provide a Configuration Container implementation
//
// *****************************************************************************************

// *****************************************************************************************
//
// Section: Import headers
//
// *****************************************************************************************

// Import C++ system headers
#include <vector>
#include <algorithm>
#include <mutex>

// Import OSAPI++ generic headers
#include "status/status.hh"
#include "util/util.hh"
#include "status/trace.hh"

// Import own module declarations
#include "configuration/defs.hh"

// Import own declarations
#include "configuration/container.hh"


namespace osapi
{

namespace configuration
{


// *****************************************************************************************
//
// Section: Module constants
//
// *****************************************************************************************

constexpr char container_default_name[]	=	"-Default-";

TRACE_CLASSNAME( container )

// *****************************************************************************************
//
// Section: Function definition
//
// *****************************************************************************************

// Class methods
const char * container::getDefaultName()
{
 return container_default_name;
}


// Definition of Constructor & Destructor

container::container( const std::string & containerName )
{
 name = containerName;
 TRACE( "Creating Container name:|", name, "|" )
}

container::~container()
{
 TRACE( "Destroying container (", name, ")" )

 for( const auto & i : ciList )	 delete i;
}

size_t container::getNumber()
{
 return ciList.size();
}

void container::getStringList( std::vector<const char *> & stringList )
{
 stringList.clear();

 for( const auto & i : ciList )
	  stringList.push_back( i->getString().c_str() );
}


void container::addList( const std::vector<item *> & list )
{
 std::string errorMsg("");

 TRACE_ENTER

 if( list.size() == 0 )
   {
	 errorMsg = "Invalid List Size";
	 throw new status( errorMsg );
   }

 // If all pre-requisites are valid, add the list to this container
 ciList.insert( std::end( ciList ), std::begin( list ), std::end( list) );

 TRACE_EXIT
}


bool container::equal( const std::string & containerName )
{
 bool ret = false;

 TRACE_POINT

 if( name == containerName )
   {
	 TRACE( "Found matching container (", name, ")" )
	 ret = true;
   }

 TRACE_EXIT

 return ret;
}



bool container::deleteItem( const std::string & itemName )
{
 bool											ret = false;
 std::vector<item *>::iterator					i   = ciList.begin();

 TRACE_ENTER

 // Make sure that the Mutex is always unlock when it goes out-of-scope
 std::lock_guard<std::mutex> guard( containerMutex );

 for( ; i != ciList.end(); i++ )
    {
      if( (*i)->equal( itemName ) )
    	  break;		// Container found, add to this container
    }

 // If a CI exists, delete CI
 if( i != ciList.end() )
   {
	 ciList.erase( i );
     ret = true;
   }

 TRACE_EXIT

 return ret;
}


void container::addItem( item * p_item )
{
 TRACE_ENTER

 if( p_item == nullptr ) throw_error( "Null pointer to item" );

 // Make sure that the Mutex is always unlock when it goes out-of-scope
 std::lock_guard<std::mutex> guard( containerMutex );

 // If all pre-requisites are valid, add the item to this container
 ciList.push_back( p_item );

 TRACE_EXIT
}

bool container::getItem( const std::string & name, item ** p_item )
{
 bool ret = false;
 TRACE_ENTER

 if( p_item == nullptr ) throw_error( "Null configurationItem pointer" );

 // Make sure that the Mutex is always unlock when it goes out-of-scope
 std::lock_guard<std::mutex> guard( containerMutex );

 for( const auto & i : ciList )
	  if( i->equal( name ) )
	    {
		  // Return address of configurationItem and not the address in the item in the vector (item is pointer to object and not the object itself)
		  *p_item = i;
		  ret = true;
		  break;
	    }

 TRACE_EXIT

 return ret;
}


std::string & container::getName()
{
	return name;
}


}	// End of namespace "configuration"
}	// End of namespace "osapi"


