/*
* Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
*
* @APPLE_OSREFERENCE_LICENSE_HEADER_START@
*
* This file contains Original Code and/or Modifications of Original Code
* as defined in and that are subject to the Apple Public Source License
* Version 2.0 (the 'License'). You may not use this file except in
* compliance with the License. The rights granted to you under the License
* may not be used to create, or enable the creation or redistribution of,
* unlawful or unlicensed copies of an Apple operating system, or to
* circumvent, violate, or enable the circumvention or violation of, any
* terms of an Apple operating system software license agreement.
*
* Please obtain a copy of the License at
* http://www.opensource.apple.com/apsl/ and read it before using this file.
*
* The Original Code and all software distributed under the License are
* distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
* EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
* Please see the License for the specific language governing rights and
* limitations under the License.
*
* @APPLE_OSREFERENCE_LICENSE_HEADER_END@
*/
/*
* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved
*
* Copyright (c) 1992, 1993
*	The Regents of the University of California.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	This product includes software developed by the University of
*	California, Berkeley and its contributors.
* 4. Neither the name of the University nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*	@(#)qsort.c	8.1 (Berkeley) 6/4/93
*/

// After all that spooky copyright stuff, Andrew Artz modified this
// woo!

#pragma once

#ifndef SWAPINIT
#define SWAPINIT(a, es) swaptype = ((char *)a - (char *)0) % sizeof(long) || \
	es % sizeof(long) ? 2 : es == sizeof(long)? 0 : 1;
#endif

/*
* Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
*/
#ifndef swapcode
#define swapcode(TYPE, parmi, parmj, n) { 		\
	long i = (n) / sizeof (TYPE); 			\
	TYPE *pi = (TYPE *) (parmi); 			\
	TYPE *pj = (TYPE *) (parmj); 			\
	do { 						\
		TYPE	t = *pi;			\
		*pi++ = *pj;				\
		*pj++ = t;				\
        } while (--i > 0);				\
}
#endif

#define swap(a, b)					\
	if (swaptype == 0) {				\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else						\
		swapfunc((char *)a, (char *)b, sizeof(T), swaptype)

#ifndef min
#define min(a, b) (a) < (b) ? a : b
#endif

#ifndef vecswap
#define vecswap(a, b, n) if ((n) > 0) swapfunc((char*) a, (char*) b, n, swaptype)
#endif

namespace nsl
{
	//
	static inline void
		swapfunc(char *a, char *b, int n, int swaptype)
	{
		if (swaptype <= 1)
			swapcode(long, a, b, n)
		else
			swapcode(char, a, b, n)
	}

	//
	template<typename T>
	static inline char *
		med3(char *a, char *b, char *c, int(*cmp)(T *, T *))
	{
		return cmp((T*)a, (T*)b) < 0 ?
			(cmp((T*)b, (T*)c) < 0 ? b : (cmp((T*)a, (T*)c) < 0 ? c : a))
			: (cmp((T*)b, (T*)c) > 0 ? b : (cmp((T*)a, (T*)c) < 0 ? a : c));
	}

	//
	template<typename T>
	void sort(T *data, size_t size, int(*cmp)(T *, T *))
	{
		char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
		int d, swaptype, swap_cnt;
		int r;

	loop:	SWAPINIT(data, sizeof(T));
		swap_cnt = 0;
		if (size < 7) {
			for (pm = (char *)data + sizeof(T); pm < (char *)data + size * sizeof(T); pm += sizeof(T))
				for (pl = pm; pl >(char *) data && cmp((T*)pl - sizeof(T), (T*)pl) > 0; pl -= sizeof(T))
					swap(pl, pl - sizeof(T));
			return;
		}
		pm = (char *)data + (size / 2) * sizeof(T);
		if (size > 7) {
			pl = (char *) data;
			pn = (char *) data + (size - 1) * sizeof(T);
			if (size > 40) {
				d = (size / 8) * sizeof(T);
				pl = med3<T>((char*)(pl), (char*) (pl + d), (char*)(pl + 2 * d), cmp);
				pm = med3<T>((char*)(pm - d), (char*) pm, (char*)(pm + d), cmp);
				pn = med3<T>((char*)(pn - 2 * d), (char*)(pn - d), (char*)(pn), cmp);
			}
			pm = med3(pl, pm, pn, cmp);
		}
		swap(data, pm);
		pa = pb = (char *)data + sizeof(T);

		pc = pd = (char *)data + (size - 1) * sizeof(T);
		for (;;) {
			while (pb <= pc && (r = cmp((T*) pb, data)) <= 0) {
				if (r == 0) {
					swap_cnt = 1;
					swap(pa, pb);
					pa += sizeof(T);
				}
				pb += sizeof(T);
			}
			while (pb <= pc && (r = cmp((T*) pc, data)) >= 0) {
				if (r == 0) {
					swap_cnt = 1;
					swap(pc, pd);
					pd -= sizeof(T);
				}
				pc -= sizeof(T);
			}
			if (pb > pc)
				break;
			swap(pb, pc);
			swap_cnt = 1;
			pb += sizeof(T);
			pc -= sizeof(T);
		}
		if (swap_cnt == 0) {  /* Switch to insertion sort */
			for (pm = (char *)data + sizeof(T); pm < (char *)data + size * sizeof(T); pm += sizeof(T))
				for (pl = pm; pl >(char *) data && cmp((T*)(pl - sizeof(T)), (T*)pl) > 0; pl -= sizeof(T))
					swap(pl, pl - sizeof(T));
			return;
		}

		pn = (char *)data + size * sizeof(T);
		r = min(pa - (char *)data, pb - pa);
		vecswap(data, pb - r, r);
		r = min((size_t)(pd - pc), pn - pd - sizeof(T));
		vecswap(pb, pn - r, r);
		if ((size_t)(r = pb - pa) > sizeof(T))
			sort<T>(data, r / sizeof(T), cmp);
		if ((size_t)(r = pd - pc) > sizeof(T)) {
			/* Iterate rather than recurse to save stack space */
			data = (T*)(pn - r);
			size = r / sizeof(T);
			goto loop;
		}
	}
}