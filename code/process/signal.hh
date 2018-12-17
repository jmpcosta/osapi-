// *****************************************************************************************
//
// File description:
//
// Author:	Joao Costa
// Purpose:	Provide signal module API
//
// *****************************************************************************************

#ifndef PROCESS_SIGNAL_HH_
#define PROCESS_SIGNAL_HH_

// *****************************************************************************************
//
// Section: Import headers
//
// *****************************************************************************************

// Standard C++ headers
#include <cstdint>
#include <csignal>

// Import OSAPI++ declarations
#include "general/general_types.hh"
#include "status/trace_macros.hh"



// *****************************************************************************************
//
// Section: Module Constant definitions
//
// *****************************************************************************************

// Redefine signal handler function (must match OSAPI and ANSI C definitions)
//typedef void (* signal_function)( int );

enum class signalSupportLevel { unsupported, ansi, posix, realTime };


// *****************************************************************************************
//
// Section: Module API declaration
//
// *****************************************************************************************

namespace osapi
{

class signal
 {
 public:
	static	bool	isSupported		( signalSupportLevel & 	);

					// Constructor & Destructor
					~signal();
					signal			( int number			);
	bool			equal			( int number			);
	int				getID			( void					);
	void 			setHandler		( sighandler_t func		);
	void 			clearHandler	( void					);
	sighandler_t	getHandler		( void					);


 private:

	// Instance variables
	int						signumber;
	sighandler_t			handler;

	TRACE_CLASSNAME_DECLARATION
};


}	// End of namespace "osapi"


#endif /* PROCESS_SIGNAL_HH_ */

