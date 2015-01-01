/* ANSI and traditional C compatibility macros
   Copyright 1991, 1992 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/*                                            

                                             

                                                   
                                                   
                          
                                      
                          
                      
                                   
                                   

                                                           

                              

                       

                                                                 
                                                                  

                                                                     
                                                                      
                                                                  
                                                   

                    

                                                   

                                                         

                                                              
                                            
                                                                     
                                                
                                                                      

                   

                                                                   
                                                                    
                                                   

                                           

                                                                     

                                    

                                           

                                                                   
                                                                
                                                    

                
                                                      
                                     
                                                     
                               
*/

#ifndef	_ANSIDECL_H

#define	_ANSIDECL_H	1


/*                                      
                                              */
/*             */


#if defined (__STDC__) || defined (_AIX) || (defined (__mips) && defined (_SYSTYPE_SVR4)) || defined(WIN32)
/*                                                                 
                                                                    
                                                */

#define	PTR		void *
#define	PTRCONST	void *CONST
#define	LONG_DOUBLE	long double

#define	AND		,
#define	NOARGS		void
#define	CONST		const
#define	VOLATILE	volatile
#define	SIGNED		signed
#define	DOTS		, ...

#define	EXFUN(name, proto)		name proto
#define	DEFUN(name, arglist, args)	name(args)
#define	DEFUN_VOID(name)		name(void)

#define PROTO(type, name, arglist)	type name arglist
#define PARAMS(paramlist)		paramlist
#define ANSI_PROTOTYPES			1

#else	/*              */

#define	PTR		char *
#define	PTRCONST	PTR
#define	LONG_DOUBLE	double

#define	AND		;
#define	NOARGS
#define	CONST
#ifndef const /*                                                          */
#define	const
#endif
#define	VOLATILE
#define	SIGNED
#define	DOTS

#define	EXFUN(name, proto)		name()
#define	DEFUN(name, arglist, args)	name arglist args;
#define	DEFUN_VOID(name)		name()
#define PROTO(type, name, arglist) type name ()
#define PARAMS(paramlist)		()

#endif	/*          */

#endif	/*            */
