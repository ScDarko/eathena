#ifndef __BASESTRING_H__
#define __BASESTRING_H__

//!! bloat marker included
//!! move back included

#include "basetypes.h"
#include "baseobjects.h"
#include "basememory.h"
#include "basealgo.h"
#include "basetime.h"
#include "basearray.h"





///////////////////////////////////////////////////////////////////////////////
// test functions
///////////////////////////////////////////////////////////////////////////////
void test_strings(void);
void test_stringbuffer(void);




///////////////////////////////////////////////////////////////////////////////
// character conversion/checking
// with specified implementations for certain types
///////////////////////////////////////////////////////////////////////////////
template <class T> inline T locase(T c) { return ::tolower( to_unsigned(c) ); }
inline char locase(char c) { if (c >= 'A' && c <= 'Z') return char(c + 32); return c; }
inline wchar_t locase(wchar_t c) { return ::towlower( to_unsigned(c) ); }

template <class T> inline T upcase(T c) { return ::toupper( to_unsigned(c) ); }
// implementation for char (and wchar)
inline char upcase(char c) { if (c >= 'a' && c <= 'z') return char(c - 32); return c; }
inline wchar_t upcase(wchar_t c) { return ::towupper( to_unsigned(c) ); }






///////////////////////////////////////////////////////////////////////////////
//!! TODO: check for integration to the template generation
//         (currently implemented as external stringtable)
#define CONSTSTRING(x) x
//#define CONSTSTRING(x) L##x		// unicode string marker






///////////////////////////////////////////////////////////////////////////////
// cstring to number
///////////////////////////////////////////////////////////////////////////////
template <class T> uint64 stringtoue(const T* str, size_t base);
template <class T> sint64 stringtoie(const T* str);
template <class T> sint64 stringtoi(const T* p);
template <class T> double stringtod(const T* p);

///////////////////////////////////////////////////////////////////////////////
// number conversion core
// don't use directly
///////////////////////////////////////////////////////////////////////////////
template <class T> const T* _itobase(sint64 value, T* buf, size_t base, size_t& len, bool _signed);



///////////////////////////////////////////////////////////////////////////////
// objects for sprintf compatible formating
// don't use directly
///////////////////////////////////////////////////////////////////////////////
template <class T, class X> class formatobj
{
	
	const T*			cfmt;
	mutable const X&	cval;
	bool				cconst;
public:
	formatobj(const T* f, const X& v) : cfmt(f), cval(v), cconst(true)	{}
	formatobj(const T* f, X& v) : cfmt(f), cval(v), cconst(false)		{}
	~formatobj() {}

	const T* fmt() const		{ return (cfmt)?cfmt:""; }
	const X& val() const		{ return cval; }
	operator const X&() const	{ return cval; }
	const formatobj<T,X>& operator=(const X& v) const
	{
		if(!cconst)
			const_cast<X&>(cval) = v;
		return *this;
	}
	operator X&()				{ return const_cast<X&>(cval); }
};




template <class T, class X> formatobj<T,X> format(const T*f, const X& v)
{
	return formatobj<T,X>(f,v);
}
template <class T, class X> formatobj<T,X> format(const T*f, X& v)
{
	return formatobj<T,X>(f,v);
}




///////////////////////////////////////////////////////////////////////////////
// predeclaration
///////////////////////////////////////////////////////////////////////////////

template <class T> class basestring;		// dynamic buffer without smart pointer
template <class T> class staticstring;		// external static buffer
template <class T> class string;			// dynamic buffer with smart pointer, copy-on-write
template <class T> class globalstring;		// dynamic buffer with smart pointer, copy-on-write
template <class T> class substring;			// contains pointer to string, implements allocator interface


///////////////////////////////////////////////////////////////////////////////
// error message
void string_error(const char*errmsg);



///////////////////////////////////////////////////////////////////////////////
// basic string interface
///////////////////////////////////////////////////////////////////////////////
template <class T=char> class stringinterface : public allocator<T>
{
protected:
	///////////////////////////////////////////////////////////////////////////
	// internal StringConstant aquire
	const T*getStringConstant(size_t i) const;

public:
	stringinterface<T>()			{}
	virtual ~stringinterface<T>()	{}
	virtual operator const T*() const =0;
	virtual const T* c_str() const =0;
	///////////////////////////////////////////////////////////////////////////
	// search functions
	///////////////////////////////////////////////////////////////////////////
	bool			findnext(const stringinterface<T>& pattern, size_t &startpos, bool ignorecase=false) const;
	vector<size_t>	findall (const stringinterface<T>& pattern, bool ignorecase=false) const;


	/////////////////////////////////////////////////////////////////
	// basic assign
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& assign(const T* c, size_t slen) = 0;
	virtual const stringinterface<T>& assign(const T& c, size_t slen) = 0;

	/////////////////////////////////////////////////////////////////
	// basic append
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& append(const T* c, size_t slen) = 0;
	virtual const stringinterface<T>& append(const T& c, size_t slen) = 0;


	/////////////////////////////////////////////////////////////////
	// change case
	/////////////////////////////////////////////////////////////////
	virtual void tolower() =0;
	virtual void toupper() =0;
	///////////////////////////////////////////////////////////////////////////
	// Trim's
	///////////////////////////////////////////////////////////////////////////
	virtual void ltrim() =0;
	virtual void rtrim() =0;
	virtual void trim() =0;
	virtual void itrim(bool removeall=false) =0;


	///////////////////////////////////////////////////////////////////////////
	// compare functions
	///////////////////////////////////////////////////////////////////////////
	virtual int compareTo(const stringinterface<T>& s) const=0;
	virtual int compareTo(const T c) const=0;
	virtual int compareTo(const T *c) const=0;

	/////////////////////////////////////////////////////////////////
	// boolean operators
	/////////////////////////////////////////////////////////////////
	virtual bool operator ==(const stringinterface<T> &s)	const { return (this->compareTo( s ) == 0); }
	virtual bool operator !=(const stringinterface<T> &s)	const { return (this->compareTo( s ) != 0); }
	virtual bool operator <=(const stringinterface<T> &s)	const { return (this->compareTo( s ) <= 0); }
	virtual bool operator >=(const stringinterface<T> &s)	const { return (this->compareTo( s ) >= 0); }
	virtual bool operator < (const stringinterface<T> &s)	const { return (this->compareTo( s ) <  0); }
	virtual bool operator > (const stringinterface<T> &s)	const { return (this->compareTo( s ) >  0); }

	virtual bool operator ==(const T c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const T *c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T *c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T *c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T *c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T *c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T *c) 	const { return (this->compareTo( c ) >  0); }

	friend bool operator ==(const T *c, const stringinterface<T> &s)	{return (s==c);}
	friend bool operator !=(const T *c, const stringinterface<T> &s)	{return (s!=c);}
	friend bool operator <=(const T *c, const stringinterface<T> &s)	{return (s>=c);}
	friend bool operator >=(const T *c, const stringinterface<T> &s)	{return (s<=c);}
	friend bool operator < (const T *c, const stringinterface<T> &s)	{return (s> c);}
	friend bool operator > (const T *c, const stringinterface<T> &s)	{return (s< c);}
};


///////////////////////////////////////////////////////////////////////////////
// basic string functions
///////////////////////////////////////////////////////////////////////////////
template<class T> inline size_t strlen(const stringinterface<T>& p)	{ return p.length(); }
template<class T> inline size_t hstrlen(const stringinterface<T>& p){ return p.length(); }

template<class T> const stringinterface<T>& toupper(stringinterface<T>& str)	{ return str.toupper(); return str; }
template<class T> const stringinterface<T>& tolower(stringinterface<T>& str)	{ return str.tolower(); return str; }
template<class T> const stringinterface<T>& ltrim(stringinterface<T>& str)		{ return str.ltrim(); return str; }
template<class T> const stringinterface<T>& rtrim(stringinterface<T>& str)		{ return str.rtrim(); return str; }
template<class T> const stringinterface<T>& trim(stringinterface<T>& str)		{ return str.trim(); return str; }
template<class T> const stringinterface<T>& itrim(stringinterface<T>& str)		{ return str.itrim(); return str; }


///////////////////////////////////////////////////////////////////////////////
// basic number conversion
///////////////////////////////////////////////////////////////////////////////
template <class T> void _itostring(stringinterface<T>& result, sint64 value, size_t base, bool _signed, size_t width=0, T padchar=0);
template <class T> void _ftostring(stringinterface<T>& result, double value, int prec, size_t width=0, T padchar=0);



///////////////////////////////////////////////////////////////////////////////
// basic string functions
///////////////////////////////////////////////////////////////////////////////
// some Unix systems do not accept NULL
inline size_t hstrlen(const char* p)		{ return p == NULL ? 0 : strlen(p); }
inline size_t hstrlen(const wchar_t* p)		{ return p == NULL ? 0 : wcslen(p); }

template<class T> const T* toupper(T* str);
template<class T> const T* tolower(T* str);
template<class T> const T* ltrim(T* str);
template<class T> const T* rtrim(T* str);
template<class T> const T* trim(T* str);
template<class T> const T* itrim(T* str, bool removeall=false);




///////////////////////////////////////////////////////////////////////////////
// basic string template implementations
// don't use this but the derived classes
///////////////////////////////////////////////////////////////////////////////
// const part
///////////////////////////////////////////////////////////////////////////////
template < class T, class A>
class TStringConst : public A, public stringinterface<T>
{
protected:

	TStringConst<T,A>() : A()							{}
	TStringConst<T,A>(size_t sz) : A(sz)				{}
	TStringConst<T,A>(T* buf, size_t sz) : A(buf, sz)	{}
public:
	virtual ~TStringConst<T,A>()						{}


	bool is_empty() const
	{
		return this->cWpp==this->cBuf; 
	}
	///////////////////////////////////////////////////////////////////////////
	// base class functions
	///////////////////////////////////////////////////////////////////////////
	virtual const T* c_str() const		
	{
		if(!this->cBuf && const_cast<TStringConst<T,A>*>(this)->checkwrite(1))
			*(const_cast<TStringConst<T,A>*>(this)->cWpp)=0; 
		return this->cBuf; 
	}
	virtual operator const T*() const	{ return this->c_str(); }
	virtual size_t length()	const		{ return this->A::length(); }
	virtual const T* begin() const		{ return this->A::begin(); }
	virtual const T* end() const		{ return this->A::end(); }
	virtual const T* final() const		{ return this->A::final(); }

	///////////////////////////////////////////////////////////////////////////
	// add operators
	// always return dynamic allocates buffers
	///////////////////////////////////////////////////////////////////////////
	template <class X> string<T> operator +(const X& t) const
	{
		return ( string<T>(*this)<< t );
	}

	///////////////////////////////////////////////////////////////////////////
	// compare functions
	///////////////////////////////////////////////////////////////////////////
	virtual int compareTo(const stringinterface<T>& s) const
	{	
		if(s.c_str() == this->c_str() )
			return 0;
		else if(s.length()>0 && this->length()>0) 
			// compare including eos
			return this->intern_cmp(this->c_str(), s.c_str(), 1+this->length());
		else if(s.length()==0 && this->length()==0)
			return 0;
		else if(s.length()==0)
			return 1;
		else
			return -1;
	}
	virtual int compareTo(const T c) const
	{	
		if(this->length()==1)
			return *this->c_str() - c;
		else if(this->c_str())
			return 1;
		else 
			return -1;
	}
	virtual int compareTo(const T *c) const
	{	
		if(c == this->c_str() )
			return 0;
		else if(c && this->length()>0)
			return this->intern_cmp(this->c_str(), c, 1+this->length());
		else if((!c || *c==0) && this->length()==0)
			return 0;
		else if((!c || *c==0))
			return 1;
		else 
			return -1;
	}

	///////////////////////////////////////////////////////////////////////////
	// finds
	///////////////////////////////////////////////////////////////////////////
	size_t find_first_of(const T& ch) const
	{
		const T *ptr  = this->begin();
		const T *eptr = this->end();
		while(ptr<=eptr)
		{
			if( *ptr == ch )
				return ptr-this->begin();
			++ptr;
		}
		return this->npos;
	}
	size_t find_last_of(const T& ch) const
	{
		const T *ptr  = this->end();
		const T *eptr = this->begin();
		while(ptr>=eptr)
		{
			if( *ptr == ch )
				return ptr-this->begin();
			--ptr;
		}
		return this->npos;
	}

	///////////////////////////////////////////////////////////////////////////
	// hash
	///////////////////////////////////////////////////////////////////////////
	size_t hash() const
	{	
		unsigned long h = 0;
		T*ptr = this->cBuf;
		while(ptr<this->cWpp)
		{
			// hash from stl
			h = 5*h + *ptr++;
			// hash used in ea
			//h = (h*33 + *ptr++) ^ (h>>24);
		}
		return size_t(h);
	}
};

template < class T=char, class A=allocator_ws_dy<T,elaborator_st<T> > >
class TString : public TStringConst<T, A>
{
	friend void test_stringbuffer(void);
protected:
	///////////////////////////////////////////////////////////////////////////
	// protected constructors, only derived can create
	///////////////////////////////////////////////////////////////////////////
	TString<T,A>() : TStringConst<T,A>()							{}
	TString<T,A>(size_t sz) : TStringConst<T,A>(sz)					{}
	TString<T,A>(T* buf, size_t sz) : TStringConst<T,A>(buf, sz)	{}

	///////////////////////////////////////////////////////////////////////////
	// M$ VisualC does not allow template generated copy/assignment operators
	// but always generating defaults if not explicitely defined
	// mixing template and explicit definition does also not work because
	// it "detects" ambiguities in this case
//	template <class A> TString<T,A>(const TString<T, A>& b) : alloc(b.length())
//	{
//		*this<<b;
//	}
	// declare standard copy constructor
	TString<T,A>(const TString<T,A>& b) : TStringConst<T,A>(b.length())
	{
		*this<<b;
	}
	// declare copy of base class types
	TString<T,A>(const stringinterface<T>& t) : TStringConst<T,A>(t.length())
	{
		*this<<t;
	}

public:
	virtual ~TString<T,A>()											{}


	///////////////////////////////////////////////////////////////////////////
	// assignment operators
	///////////////////////////////////////////////////////////////////////////
// M$ VisualC does not allow template generated copy/assignment operators
// but always generating defaults if not explicitely defines
// mixing template and explicit definition does also not work because
// it "detects" ambiguities in this case
//	template <class X> const TString<T,A>& operator =(const X& t)
//	{
//		this->cWpp = this->cBuf;
//		return *this<<t;
//	}

	// only default copy assignment usable
	const TString<T,A>& operator=(const TString<T,A>& t)	{ this->cWpp = this->cBuf; return this->operator<<(t); }

	const TString<T,A>& operator=(const stringinterface<T>& t){ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const T* t)				{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const T t)				{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const int t)				{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const unsigned int t)		{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const long t)				{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const unsigned long t)	{ this->cWpp = this->cBuf; return this->operator<<(t); }
	const TString<T,A>& operator=(const double t)			{ this->cWpp = this->cBuf; return this->operator<<(t); }


	///////////////////////////////////////////////////////////////////////////
	// standard functions
	///////////////////////////////////////////////////////////////////////////
	void clear()				
	{
		this->cWpp=this->cBuf; 
		if(this->cWpp) *this->cWpp=0;
	}
	void empty()
	{
		this->clear(); 
	}
	// remove everything from ix for sz length
	void clear(size_t inx, size_t len)
	{
		if( this->cBuf+inx < this->cWpp )
		{
			if(this->cBuf+inx+len >= this->cWpp)
				len = this->cWpp-this->cBuf-inx;
			else
				this->intern_move(this->cBuf+inx, this->cBuf+inx+len, this->cWpp-this->cBuf-inx-len);
			this->cWpp -= len;
			*this->cWpp = 0;
		}
	}
	// remove everything exept from 0 for sz length
	void truncate(size_t sz)
	{
		if( this->cBuf+sz < this->cWpp )
		{	// truncate
			this->cWpp = this->cBuf+sz;
			*this->cWpp = 0;
		}
		else
		{	// expand by padding spaces
			if( this->checkwrite( sz-(this->cWpp-this->cBuf) ) )
			{
				while( this->cBuf+sz > this->cWpp )
					*this->cWpp++ = (T)(' ');
				if(this->cWpp) *this->cWpp = 0;
			}
		}
	}
	// remove everything exept from ix for sz length
	void truncate(size_t ix, size_t sz)
	{
		if(ix==0)
		{	// normal truncate
			truncate(sz);
		}
		else if( this->cBuf+ix > this->cWpp )
		{	// cut all off
			this->cWpp = this->cBuf;
			*this->cWpp = 0;
		}
		else if( this->cBuf+ix+sz <= this->cWpp )  
		{	// move and truncate
			this->intern_move(this->cBuf, this->cBuf+ix, sz);
			this->cWpp = this->cBuf+sz;
			*this->cWpp = 0;
		}
		else
		{	// move and expand
			this->intern_move(this->cBuf, this->cBuf+ix, this->cWpp-this->cBuf-ix);
			this->cWpp -= ix;
			if( this->checkwrite( sz-(this->cWpp-this->cBuf) ) )
			{
				while( this->cBuf+sz > this->cWpp )
					*this->cWpp++ = (T)(' ');
			}
			if(this->cWpp) *this->cWpp = 0;
		}
	}
	// removes sz elements from the end
	bool strip(size_t sz)
	{
		if( this->cBuf+sz > this->cWpp )
		{	// cut all off
			this->cWpp = this->cBuf;
			if(this->cWpp) *this->cWpp = 0;
		}
		else
		{	
			this->cWpp-=sz;
			*this->cWpp = 0;
		}
		return true;
	}

	bool checksize(size_t sz)	
	{
		return (this->cBuf+sz>this->cEnd) ? checkwrite(sz-(this->cEnd-this->cBuf)) : true; 
	}
	///////////////////////////////////////////////////////////////////////////
	// array access
	///////////////////////////////////////////////////////////////////////////
	const T& operator[](size_t inx) const
	{
	#ifdef CHECK_BOUNDS
		if( this->cBuf+inx > this->cWpp )
		{
			string_error("string: access out of bound");
	#ifndef CHECK_EXCEPTIONS
			// provide some fallback in case of out of bound
			static T dummy;
			return dummy=0;
	#endif
		}
	#endif
		return this->cBuf[inx];
	}
	T& operator[](size_t inx)
	{
		// merge with exeption code
		if( this->cEnd < this->cWpp )	// const string
		{
			static T dummy;
			return dummy=0;
		}
	#ifdef CHECK_BOUNDS
		if( this->cBuf+inx > this->cWpp )
		{
			string_error("string: access out of bound");
	#ifndef CHECK_EXCEPTIONS
			// provide some fallback in case of out of bound
			static T dummy;
			return dummy=0;
	#endif
		}
	#endif
		return this->cBuf[inx];
	}
	const T& operator[](int inx) const	{ return this->operator[]( (size_t)inx ); }
	T& operator[](int inx)				{ return this->operator[]( (size_t)inx ); }
	///////////////////////////////////////////////////////////////////////////
	// assignment function
	///////////////////////////////////////////////////////////////////////////
	template<class X> const TString<T,A>& assign(const X& t)
	{
		this->cWpp = this->cBuf;
		return this->operator<<(t);
	}
	virtual const stringinterface<T>& assign(const T* c, size_t slen)
	{
		this->cWpp = this->cBuf;
		if( c && this->checkwrite(slen) )
		{
			const T* epp=c+slen;
			while( *c && c<epp )
				*this->cWpp++ = *c++;
		}
		if( this->cWpp ) *this->cWpp=0;
		return *this;
	}
	virtual const stringinterface<T>& assign(const T& ch, size_t slen)
	{
		this->cWpp = this->cBuf;
		if(ch)
		{	
			if( this->checkwrite(slen) )
			{
				const T* epp=this->cWpp+slen;
				while( this->cWpp < epp )
					*this->cWpp++ = ch;
			}
		}
		if( this->cWpp ) *this->cWpp=0;
		return *this;
	}
	const TString<T,A>& assign(const T* c1, size_t len1, const T* c2, size_t len2)
	{
		if( !c1 || len1 <= 0 )
			assign(c2, len2);
		else if ( !c2 || len2 <= 0 )
			assign(c1, len1);
		else if( this->checkwrite(len1 + len2) )
		{
			this->cWpp = this->cBuf;

			const T* epp=c1+len1;
			while( *c1 && c1 < epp )
				*this->cWpp++ = *c1++;

			epp=c2+len2;
			while( *c2 && c2 < epp )
				*this->cWpp++ = *c2++;

			if(this->cWpp) *this->cWpp=0;
		}
		return *this;
	}
	const TString<T,A>& assign_tolower(const T* c, size_t slen=~0)
	{
		this->cWpp = this->cBuf;
		if(c)
		{
			const T* epp = (slen==~0) ? (const T*)(~((size_t)0)) : epp=c+slen;
			while(*c && c<epp && this->checkwrite(1) )
				*this->cWpp++ = locase(*c++);
		}
		if( this->cWpp ) *this->cWpp=0;
		return *this;
	}
	const TString<T,A>& assign_toupper(const T* c, size_t slen=~0)
	{
		this->cWpp = this->cBuf;
		if(c)
		{
			const T* epp = (slen==~0) ? (const T*)(~((size_t)0)) : epp=c+slen;
			while(*c && c<epp && this->checkwrite(1) )
				*this->cWpp++ = upcase(*c++);
		}
		if( this->cWpp ) *this->cWpp=0;
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// append function
	///////////////////////////////////////////////////////////////////////////
	template<class X> TString<T,A>& append(const X& t)
	{
		return this->operator<<(t);
	}
	///////////////////////////////////////////////////////////////////////////
	// append with length
	///////////////////////////////////////////////////////////////////////////
	TString<T,A>& append(const stringinterface<T>& t, size_t slen)
	{
		if( slen > t.length() )
			slen = t.length();
		if(slen)
		{
			if( this->cWpp+slen<this->cEnd || this->checkwrite( slen ) )
			{
				this->intern_copy(this->cWpp, t, slen);
				this->cWpp += slen;
				if(this->cWpp) *this->cWpp=0;
			}
		}
		return *this;
	}
	virtual const stringinterface<T>& append(const T* c, size_t slen)
	{
		if(c)
		{
			const T* epp = c+slen;
			while(*c && c<epp && this->checkwrite(1) )
				*this->cWpp++ = *c++;
			if(this->cWpp) *this->cWpp=0;
		}
		return *this;
	}
	virtual const stringinterface<T>& append(const T& ch, size_t slen)
	{
		if(ch) // dont append a eos
		{
			if( this->cWpp+slen<this->cEnd || this->checkwrite( slen ) )
			{
				T* epp = this->cWpp+slen;
				while(this->cWpp<epp)
					*this->cWpp++ = ch;
				if(this->cWpp) *this->cWpp = 0;
			}
		}
		return *this;
	}
	virtual const stringinterface<T>& append(const T ch)
	{
		if(ch) // dont append a eos
		{
			if( this->cWpp+1<this->cEnd || this->checkwrite( 1 ) )
			{
				*this->cWpp++ = ch;
				*this->cWpp = 0;
			}
		}
		return *this;
	}
	/////////////////////////////////////////////////////////////////
	// insert
	/////////////////////////////////////////////////////////////////
	TString<T,A>& insert(size_t pos, const stringinterface<T>& t, size_t slen=~0)
	{
		if( pos > this->length() )
			return this->append(t, slen);
		else
		{
			if( slen > t.length() )
				slen = t.length();
			if( this->cWpp+slen<this->cEnd || this->checkwrite( slen ) )
			{
				this->intern_move(this->cBuf+pos+slen, this->cBuf+pos, this->cWpp-this->cBuf-pos);
				this->intern_copy(this->cBuf+pos, t, slen);

				this->cWpp += slen;
				*this->cWpp = 0;
			}
		}
		return *this;
	}
	TString<T,A>& insert(size_t pos, const T* c, size_t slen=~0)
	{
		if(c)
		{
			if( pos > this->length() )
			{
				this->append(c, slen);
			}
			else
			{
				const T* ep =c;
				while(*ep) ep++;
				if( slen > (size_t)(ep-c) )
					slen = ep-c;
				if( this->cWpp+slen<this->cEnd || this->checkwrite( slen ) )
				{
					this->intern_move(this->cBuf+pos+slen, this->cBuf+pos, this->cWpp-this->cBuf-pos);
					this->intern_copy(this->cBuf+pos, c, this->cWpp-this->cBuf-pos);

					this->cWpp += slen;
					*this->cWpp = 0;
				}
			}
		}
		return *this;
	}
	TString<T,A>& insert(size_t pos, const T ch, size_t slen=1)
	{
		if(ch) // dont append a eos
		{
			if( pos > this->length() )
			{
				this->append(ch, slen);
			}
			else
			{
				if( this->cWpp+slen<this->cEnd || this->checkwrite( slen ) )
				{
					T* ipp = this->cBuf+pos;
					T* epp = this->cBuf+pos+slen;
					this->intern_move(epp, ipp, this->cWpp-this->cBuf-pos);
					while(ipp<epp)
						*ipp++ = ch;

					this->cWpp += slen;
					*this->cWpp = 0;
				}
			}
		}
		return *this;
	}
	/////////////////////////////////////////////////////////////////
	// replace
	/////////////////////////////////////////////////////////////////
	TString<T,A>& replace(size_t pos, size_t tlen, const stringinterface<T>& t, size_t slen=~0)
	{
		if( pos > this->length() )
		{
			this->append(t, slen);
		}
		else
		{
			if( pos+tlen > this->length() )
				tlen = this->length()-pos;
			if( slen > t.length() )
				slen = t.length();
			if( slen<=tlen || this->checkwrite( slen-tlen ) )
			{
				this->intern_move(this->cBuf+pos+slen, this->cBuf+pos+tlen, this->cWpp-this->cBuf-pos-tlen);
				this->intern_copy(this->cBuf+pos, t, slen);

				this->cWpp += slen-tlen;
				if(this->cWpp) *this->cWpp = 0;
			}
		}
		return *this;
	}
	TString<T,A>& replace(size_t pos, size_t tlen, const T* c, size_t slen=~0)
	{
		if(c)
		{
			if( pos > this->length() )
			{
				this->append(c, slen);
			}
			else
			{
				if( pos+tlen > this->length() )
					tlen = this->length()-pos;

				const T* ep =c;
				while(*ep) ep++;
				if( slen > (size_t)(ep-c) )
					slen = ep-c;
				
				if( slen<=tlen || this->checkwrite( slen-tlen ) )
				{
					this->intern_move(this->cBuf+pos+slen, this->cBuf+pos+tlen, this->cWpp-this->cBuf-pos-tlen);
					this->intern_copy(this->cBuf+pos, c, slen);

					this->cWpp += slen-tlen;
					if(this->cWpp) *this->cWpp = 0;
				}
			}
		}
		return *this;
	}
	TString<T,A>& replace(size_t pos, size_t tlen, const T ch, size_t slen=1)
	{
		if(ch) // dont append a eos
		{
			if( pos > this->length() )
			{
				this->append(ch, slen);
			}
			else
			{
				if( pos+tlen > this->length() )
					tlen = this->length()-pos;
				
				if( slen<=tlen || this->checkwrite( slen-tlen ) )
				{
					T* ipp = this->cBuf+pos;
					T* epp = this->cBuf+pos+slen;
					this->intern_move(epp, ipp+tlen, this->cWpp-this->cBuf-pos-tlen);
					while(ipp<epp)
						*ipp++ = ch;

					this->cWpp += slen-tlen;
					if(this->cWpp) *this->cWpp = 0;
				}
			}
		}
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// add-assignment operators
	///////////////////////////////////////////////////////////////////////////
	template <class X> const TString<T,A>& operator +=(const X& t)
	{
		return this->operator<<(t);
	}


	///////////////////////////////////////////////////////////////////////////
	// operator realisations for supported types
	///////////////////////////////////////////////////////////////////////////
	template <class X> TString<T,A>& operator <<(const TString<T,X>& sb)
	{
		if(sb.length())
		{
			if( this->cWpp+sb.length()<this->cEnd || this->checkwrite(sb.length()) )
			{
				this->intern_copy(this->cWpp, sb, sb.length());
				this->cWpp += sb.length();
				if(this->cWpp) *this->cWpp=0;
			}
		}
		return *this;
	}
	TString<T,A>& operator <<(const stringinterface<T>& t)
	{
		if( t.length() )
		{
			if( this->cWpp+t.length()<this->cEnd || this->checkwrite( t.length() ) )
			{
				this->intern_copy(this->cWpp, t, t.length());
				this->cWpp += t.length();
				if(this->cWpp) *this->cWpp=0;
			}
		}
		return *this;
	}
	TString<T,A>& operator <<(const T* ip)
	{
		if(ip)
		{
			while( *ip && this->checkwrite(1) )
				*this->cWpp++ = *ip++;
			if(this->cWpp) *this->cWpp=0;
		}
		return *this;
	}
	TString<T,A>& operator <<(const T ch)
	{
		if( ch && this->checkwrite(1) )
		{
			*this->cWpp++ = ch;
			*this->cWpp=0;
		}
		return *this;
	}
	TString<T,A>& operator <<(const int v)
	{
		_itostring<T>(*this, sint64(v), 10, true,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const unsigned int v)
	{
		_itostring<T>(*this, uint64(v), 10, false,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const long v)
	{
		_itostring<T>(*this, sint64(v), 10, true,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const unsigned long v)
	{
		_itostring<T>(*this, uint64(v), 10, false,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const int64 v)
	{
		_itostring<T>(*this, sint64(v), 10, true,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const uint64 v)
	{
		_itostring<T>(*this, uint64(v), 10, false,  0, ' ');
		return *this;
	}
	TString<T,A>& operator <<(const double v)
	{
		_ftostring<T>(*this, v, 6, 0, ' ');
		return *this;
	}

	template<class X> TString<T,A>& operator <<(const formatobj<T,X>& fmt)
	{
		// ignore all formatings
		//!! TODO: add correct format pre-parser into the format object and the appropiate print function
		this->append( fmt.val() );
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// change case
	///////////////////////////////////////////////////////////////////////////
	virtual void tolower()
	{
		T* ipp = this->cBuf;
		while( ipp < this->cWpp )
		{
			*ipp = locase(*ipp);
			ipp++;
		}
	}
	virtual void toupper()
	{
		T* ipp = this->cBuf;
		while( ipp < this->cWpp )
		{
			*ipp = upcase(*ipp);
			ipp++;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	// trim's
	///////////////////////////////////////////////////////////////////////////
	// remove whitespaces from left side
	virtual void ltrim()
	{
		T* ipp = this->cBuf;
		while( ipp < this->cWpp && stringcheck::isspace(*ipp) )
			ipp++;
		if(ipp!=this->cBuf)
		{
			this->intern_move(this->cBuf, ipp, this->cWpp-ipp);
			*this->cWpp=0;
		}
	}
	// remove whitespaces from right side
	virtual void rtrim()
	{
		T* ipp = this->cWpp-1;
		T* kpp = this->cWpp-1;
			
		while( ipp>this->cBuf && stringcheck::isspace(*ipp) )
			ipp--;
		if( ipp != kpp )
		{
			this->cWpp=ipp+1;
			*this->cWpp=0;
		}
	}
	// remove whitespaces from both sides
	virtual void trim()
	{
		T *src=this->cBuf, *tar=this->cBuf, *mk=NULL;
		while(*src && stringcheck::isspace(*src) )
			src++;
		while(*src)
		{
			mk = ( stringcheck::isspace(*src) )?mk?mk:tar:NULL;
			*tar++ = *src++;
		}
		this->cWpp = (mk) ? mk : tar;
		if(this->cWpp) *this->cWpp=0;
	}
	// remove whitespaces from both sides, combine or remove speces inside
	virtual void itrim(bool removeall=false)
	{
		T *src=this->cBuf, *tar=this->cBuf, mk=0;
		while(*src && stringcheck::isspace(*src) )
			src++;
		while(*src)
		{
			if( stringcheck::isspace(*src) )
				mk=*src, src++;
			else
			{
				if( mk && !removeall )
					*tar++=mk, mk=0;
				*tar++ = *src++;
			}
		}
		this->cWpp = tar;
		if(this->cWpp) *this->cWpp=0;
	}
};


///////////////////////////////////////////////////////////////////////////////
// basestring class
// implements a dynamic in-place buffer
// type construction always copies the  data to a newly created buffer
///////////////////////////////////////////////////////////////////////////////
template <class T=char> class basestring : public TString< T, allocator_ws_dy<T, elaborator_st<T> > >
{
public:
	basestring()
	{}
	basestring<T>(const basestring<T>& t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	basestring<T>(const stringinterface<T>& t) : TString< T, allocator_ws_dy<T,elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	basestring<T>(const char* t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	explicit basestring<T>(char t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	explicit basestring<T>(int t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	explicit basestring<T>(unsigned int t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}
	explicit basestring<T>(double t) : TString< T, allocator_ws_dy<T, elaborator_st<T> > >()
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
	}

	virtual ~basestring()	{}


	///////////////////////////////////////////////////////////////////////////
	// assignments
	///////////////////////////////////////////////////////////////////////////
	template<class X> const basestring<T>& operator=(const X& t)
	{
		this->TString< T, allocator_ws_dy<T, elaborator_st<T> > >::assign(t);
		return *this;
	}
};

///////////////////////////////////////////////////////////////////////////////
// staticstring class
// implements an external static buffer
// type construction only possible with either a buffer which is used for writing
// or with a const cstring which is then used as a constant without write possibility
///////////////////////////////////////////////////////////////////////////////
template<class T=char> class staticstring : public TString< T, allocator_ws_st<T, elaborator_st<T> > >
{
	staticstring<T>();
	staticstring<T>(const staticstring<T>&);
	const staticstring& operator=(const staticstring& t);
public:
	staticstring<T>(T* buf, size_t sz)
		: TString< T, allocator_ws_st<T, elaborator_st<T> > >(buf, sz)
	{}
	virtual ~staticstring()	{}

	///////////////////////////////////////////////////////////////////////////
	// array access
	///////////////////////////////////////////////////////////////////////////
	virtual const T* c_str() const		
	{
		if(!this->cBuf && const_cast<staticstring<T>*>(this)->checkwrite(1))
			*this->cWpp=0; 
		return this->cBuf; 
	}

	///////////////////////////////////////////////////////////////////////////
	// assignments
	///////////////////////////////////////////////////////////////////////////
	template<class X> const staticstring<T>& operator=(const X& t)
	{
		this->TString< T, allocator_ws_st<T, elaborator_st<T> > >::assign(t);
		return *this;
	}
};

///////////////////////////////////////////////////////////////////////////////
// staticstring class
// implements an external static buffer
// type construction only possible with either a buffer which is used for writing
// or with a const cstring which is then used as a constant without write possibility
///////////////////////////////////////////////////////////////////////////////
template<class T=char> class conststring : public TStringConst< T, allocator_ws_st<T, elaborator_st<T> > >
{
	conststring<T>();
	
public:
	explicit conststring<T>(const T* cstr)
		: TStringConst< T, allocator_ws_st<T, elaborator_st<T> > >(const_cast<T*>(cstr), 0)
	{	// set up a zero-length buffer, 
		// where the length pointer is set at position
		if(cstr) this->cWpp+=hstrlen(cstr);
	}
	
	virtual ~conststring()	{}

	// copy/assign
	conststring<T>(const conststring<T>& c)
	{
		this->cBuf= c.cBuf;
		this->cWpp= c.cWpp;
		this->cEnd= c.cEnd;
	}
	const conststring& operator=(const conststring& c)
	{
		this->cBuf= c.cBuf;
		this->cWpp= c.cWpp;
		this->cEnd= c.cEnd;
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// supported assignments
	// be careful with assigning stuff here
	///////////////////////////////////////////////////////////////////////////
	const conststring& operator=(const char* t)
	{	// set up a zero-length buffer, 
		this->cBuf = this->cWpp = this->cEnd = const_cast<T*>(t);
		if(t) this->cWpp+=hstrlen(t);
		return *this;
	}
private:
	///////////////////////////////////////////////////////////////////////////
	// prevent direct asignment of stringclass content
	const conststring& operator=(const stringinterface<T>& t)
	{ }
public:
	///////////////////////////////////////////////////////////////////////////
	// standard functions
	///////////////////////////////////////////////////////////////////////////
	void clear()
	{
		this->cWpp=this->cBuf=this->cEnd=NULL; 
	}

	///////////////////////////////////////////////////////////////////////////
	// base class functions
	///////////////////////////////////////////////////////////////////////////
	virtual const T* c_str() const		
	{
		if(!this->cBuf)
			const_cast<conststring<T>*>(this)->cBuf=
			const_cast<conststring<T>*>(this)->cWpp=
			const_cast<conststring<T>*>(this)->cEnd=
			const_cast<T*>(this->getStringConstant(0));
		return this->cBuf;
	}
	///////////////////////////////////////////////////////////////////////////
	// array access
	///////////////////////////////////////////////////////////////////////////
	const T operator[](size_t inx) const
	{
		if( this->cBuf+inx > this->cWpp )
			return 0;
		else
			return this->cBuf[inx];
	}
	T operator[](size_t inx)
	{
		if( this->cBuf+inx > this->cWpp )
			return 0;
		else
			return this->cBuf[inx];
	}
	const T operator[](int inx) const	{ return this->operator[]( (size_t)inx ); }
	T operator[](int inx)				{ return this->operator[]( (size_t)inx ); }

private:
	/////////////////////////////////////////////////////////////////
	// implementation for unsupported interface functions

	/////////////////////////////////////////////////////////////////
	// basic assign
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& assign(const T* c, size_t slen)	{ return *this; }
	virtual const stringinterface<T>& assign(const T& c, size_t slen)	{ return *this; }

	/////////////////////////////////////////////////////////////////
	// basic append
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& append(const T* c, size_t slen)	{ return *this; }
	virtual const stringinterface<T>& append(const T& c, size_t slen)	{ return *this; }

	/////////////////////////////////////////////////////////////////
	// change case
	/////////////////////////////////////////////////////////////////
	virtual void tolower() {}
	virtual void toupper() {}
	///////////////////////////////////////////////////////////////////////////
	// Trim's
	///////////////////////////////////////////////////////////////////////////
	virtual void ltrim() {}
	virtual void rtrim() {}
	virtual void trim() {}
	virtual void itrim(bool removeall=false) {}
};




///////////////////////////////////////////////////////////////////////////////
// main string class
// derives internally from basestring and inherits it's dynamic buffer
// additionally it implements a smart pointer with copy-on-write semantic
// so that string copy operations are basically reduced to copying a pointer
///////////////////////////////////////////////////////////////////////////////
template <class T=char> class string : public stringinterface<T>
{
	friend class substring<T>;
private:
	/////////////////////////////////////////////////////////////////
	// smart pointer base class
	/////////////////////////////////////////////////////////////////
	class ptrstring : public basestring<T>
	{
	private:
		friend class string<T>;

		// reference counter
		unsigned int cRefCnt;

		// construction
		ptrstring() : cRefCnt(1)	{}
		ptrstring(const ptrstring& b) : basestring<T>(b), cRefCnt(1)	{}
		ptrstring(const basestring<T>& b) : basestring<T>(b), cRefCnt(1)	{}

	public:
		virtual ~ptrstring()	{}

	private:
		template<class X> const ptrstring& operator=(const X& t)
		{
			this->basestring<T>::operator=(t);
			return *this;
		}

		// access on the smart pointer
		ptrstring* aquire()
		{
			atomicincrement( &cRefCnt );
			return this;
		}
		ptrstring* release()
		{
			if( atomicdecrement( &cRefCnt ) == 0 )
				delete this;
			return NULL;
		}
	};
protected:
	/////////////////////////////////////////////////////////////////
	// the smart pointer itself
	/////////////////////////////////////////////////////////////////
	ptrstring *cCountObj;

	/////////////////////////////////////////////////////////////////
	// allocation of the pointer
	/////////////////////////////////////////////////////////////////
	virtual ptrstring *createpointer() const throw()
	{
		return new ptrstring;
	}
	virtual ptrstring *createpointer(const ptrstring& old) const throw()
	{
		return new ptrstring(old);
	}
	/////////////////////////////////////////////////////////////////
	// virtual access functions to the smart pointer
	/////////////////////////////////////////////////////////////////
	// !!copy-on-write
	virtual const basestring<T>& readaccess() const	
	{ 
		this->checkpointer();
		return *this->cCountObj;
	}
	virtual       basestring<T>& writeaccess(bool keep=true)
	{
		if(!this->cCountObj)
			this->cCountObj = createpointer();
		else
			this->make_unique(keep);
		return *this->cCountObj;
	}
	/////////////////////////////////////////////////////////////////
	// aquire/release the pointer
	/////////////////////////////////////////////////////////////////
	void acquire(const string<T>& r) throw()
	{	// check if not already pointing to the same object
		if( this->cCountObj != r.cCountObj ||  NULL==this->cCountObj )
		{	// save the current pointer
			ptrstring *old = this->cCountObj;
			// aquite and increment the given pointer
			if( NULL==r.cCountObj )
			{	// new empty counter to link both pointers
				const_cast<string<T>&>(r).cCountObj = this->createpointer();
			}
			this->cCountObj = r.cCountObj->aquire();
			// release the old thing
			if(old) old->release();
		}
	}
	void release()
	{	// decrement the count, clear the handle
		if(this->cCountObj) this->cCountObj = this->cCountObj->release();
	}
	/////////////////////////////////////////////////////////////////
	// control of the smart pointer
	/////////////////////////////////////////////////////////////////
	void checkpointer() const
	{
		if(!this->cCountObj)
			const_cast< string* >(this)->cCountObj = this->createpointer();
		// no need to aquire, is done on reference creation
	}
	bool isunique()	const throw()
	{
		return (this->cCountObj) ? (this->cCountObj->cRefCnt == 1):true;
	}
	bool make_unique(bool keep=true)
	{
		if( !this->isunique() )
		{
			ptrstring *cnt = (this->cCountObj && keep) ? 
								this->createpointer(*this->cCountObj) : 
								this->createpointer();
			this->release();
			this->cCountObj = cnt;
		}
		return true;
	}

public:
	/////////////////////////////////////////////////////////////////
	// public pointer functions
	/////////////////////////////////////////////////////////////////
	const size_t getRefCount() const
	{
		return (this->cCountObj) ? this->cCountObj->cRefCnt : 1; 
	}
	bool is_sameRef(const string<T>& str) const
	{
		return ( this->cCountObj && this->cCountObj==str.cCountObj );
	}
	bool is_sameRef(const substring<T>& sub) const
	{
		return ( this->cCountObj && sub.cString && this->cCountObj==sub.cString->cCountObj );
	}
	T* unique()
	{
		// force calling string::writeaccess, 
		// which creates a unique object
		// we then can return also a writable pointer to the string data
		// but don't try to change the size of the string by modifying the eos!!
		return const_cast<T*>(this->string::writeaccess().c_str());
	}
	/////////////////////////////////////////////////////////////////
	// direct access to the underlying class
	virtual basestring<T>& operator*()	throw()				{ return this->writeaccess(); }
	virtual operator const basestring<T>&() const throw()	{ return this->readaccess();  }


	/////////////////////////////////////////////////////////////////
	// construction/destruction
	/////////////////////////////////////////////////////////////////
	string<T>() : cCountObj(NULL)					{  }
	string<T>(const string& r) : cCountObj(NULL)	{ this->acquire(r); }
	const string<T>& operator=(const string<T>& r)	{ this->acquire(r); return *this; }
	virtual ~string<T>()							{ this->release(); }

	/////////////////////////////////////////////////////////////////
	// type construction
	/////////////////////////////////////////////////////////////////
	string<T>(const stringinterface<T>& t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	string<T>(const substring<T>& t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	string<T>(const T* t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	string<T>(const T* t, size_t sz) : cCountObj(NULL)
	{
		this->writeaccess().assign(t, sz);
	}
	explicit string<T>(T t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	explicit string<T>(int t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	explicit string<T>(unsigned int t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}
	explicit string<T>(double t) : cCountObj(NULL)
	{
		this->writeaccess().assign(t);
	}

	/////////////////////////////////////////////////////////////////
	// assignment operators
	/////////////////////////////////////////////////////////////////
	const string<T>& operator =(const stringinterface<T>& t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(const substring<T>& sub)
	{
		if( this->is_sameRef(sub) )
			this->writeaccess().truncate(sub.cPos, sub.cLen);
		else
			this->writeaccess(false).assign(sub);
		return *this;
	}
	const string<T>& operator =(const char* t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(char t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(int t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(unsigned int t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(long t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(unsigned long t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const string<T>& operator =(double t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	/////////////////////////////////////////////////////////////////
	// concatination assignment operators
	/////////////////////////////////////////////////////////////////
	const string<T>& operator +=(const stringinterface<T>& t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(const string<T>& t)
	{
		this->writeaccess() += t.readaccess();
		return *this;
	}
	const string<T>& operator +=(const char* t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(char t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(int t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(unsigned int t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(short t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(unsigned short t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(long t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(unsigned long t)
	{
		this->writeaccess() += t;
		return *this;
	}
	const string<T>& operator +=(double t)
	{
		this->writeaccess() += t;
		return *this;
	}
	/////////////////////////////////////////////////////////////////
	// add operators 
	// for right side operations
	/////////////////////////////////////////////////////////////////
	string<T> operator +(const stringinterface<T>& t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(const string<T>& t)
	{
		string<T> a(*this);
		a.writeaccess() += t.readaccess();
		return a;
	}
	string<T> operator +(const char* t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(char t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(int t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(unsigned int t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(short t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(unsigned short t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(long t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(unsigned long t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}
	string<T> operator +(double t)
	{
		string<T> a(*this);
		a.writeaccess() += t;
		return a;
	}

	/////////////////////////////////////////////////////////////////
	// c++ style piping
	/////////////////////////////////////////////////////////////////
	string<T>& operator <<(const stringinterface<T>& t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(const string<T>& t)
	{
		this->writeaccess() << t.readaccess();
		return *this;
	}
	string<T>& operator <<(const char* t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(char t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(int t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(unsigned int t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(short t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(unsigned short t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(long t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(unsigned long t)
	{
		this->writeaccess() << t;
		return *this;
	}
	string<T>& operator <<(double t)
	{
		this->writeaccess() << t;
		return *this;
	}
	template<class X> string<T>& operator <<(const formatobj<T,X>& t)
	{
		this->writeaccess() << t;
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// assignment function
	///////////////////////////////////////////////////////////////////////////
	template<class X> string<T>& assign(const X& t)
	{
		this->writeaccess().assign(t);
		return *this;
	}
	string<T>& assign(const string<T>& s, size_t len)
	{
		if( this->is_sameRef(s) )
			if(len<s.length()) this->writeaccess().truncate(s, len);
		else
			this->writeaccess().assign(s, len);
		return *this;
	}
	virtual const stringinterface<T>& assign(const T* c, size_t len)
	{
		this->writeaccess().assign(c, len);
		return *this;
	}
	virtual const stringinterface<T>& assign(const T& ch, size_t slen)
	{
		this->writeaccess().assign(ch, slen);
		return *this;
	}
	string<T>& assign(const T ch)
	{
		this->writeaccess().assign(ch);
		return *this;
	}
	string<T>& assign(const string<T>& s1, size_t len1, const string<T>& s2, size_t len2)
	{
		if( this->is_sameRef(s1) && this->is_sameRef(s2) )
		{	// force a new object in case we do self-assignments
			basestring<T> u(s1);
			this->writeaccess().assign(u, len1, u, len2);
		}
		else if( this->is_sameRef(s1) )
		{
			if(len1<s1.length()) this->writeaccess().truncate(s1, len1);
			this->writeaccess().append(s2, len2);
		}
		else if( this->is_sameRef(s2) )
		{
			if(len2<s2.length()) this->writeaccess().truncate(s2, len2);
			this->writeaccess().insert(0, s1, len1);
		}
		else
		{
			this->writeaccess().assign(s1, len1, s2, len2);
		}
	}
	string<T>& assign(const string<T>& s1, size_t len1, const T* c2, size_t len2)
	{
		if( this->is_sameRef(s1) )
		{
			if(len1<s1.length()) this->writeaccess().truncate(s1, len1);
			this->writeaccess().append(c2, len2);
		}
		else
		{
			this->writeaccess().assign(s1, len1, c2, len2);
		}
	}
	string<T>& assign(const T* c1, size_t len1, const string<T>& s2, size_t len2)
	{
		if( this->is_sameRef(s2) )
		{
			if(len2<s2.length()) this->writeaccess().truncate(s2, len2);
			this->writeaccess().insert(0, c1, len1);
		}
		else
		{
			this->writeaccess().assign(c1, len1, s2, len2);
		}
	}
	string<T>& assign(const T* c1, size_t len1, const T* c2, size_t len2)
	{
		this->writeaccess().assign(c1, len1, c2, len2);
		return *this;
	}
	string<T>& assign_tolower(const T* c, size_t len=~0)
	{
		this->writeaccess().assign_tolower(c, len);
		return *this;
	}
	string<T>& assign_toupper(const T* c, size_t len=~0)
	{
		this->writeaccess().assign_toupper(c, len);
		return *this;
	}
	///////////////////////////////////////////////////////////////////////////
	// append function
	///////////////////////////////////////////////////////////////////////////
	template<class X> string<T>& append(const X& t)
	{
		this->writeaccess().append(t);
		return *this;
	}
	/////////////////////////////////////////////////////////////////
	// append with length
	/////////////////////////////////////////////////////////////////
	string<T>& append(const stringinterface<T>& t, size_t slen=~0)
	{
		this->writeaccess().append(t, slen);
		return *this;
	}
	virtual const stringinterface<T>& append(const T* c, size_t slen=~0)
	{
		this->writeaccess().append(c, slen);
		return *this;
	}
	virtual const stringinterface<T>& append(const T& ch, size_t slen)
	{
		this->writeaccess().append(ch, slen);
		return *this;
	}

	/////////////////////////////////////////////////////////////////
	// insert
	/////////////////////////////////////////////////////////////////
	string<T>& insert(size_t pos, const stringinterface<T>& t, size_t slen=~0)
	{
		this->writeaccess().insert(pos, t, slen);
		return *this;
	}
	string<T>& insert(size_t pos, const substring<T>& sub, size_t slen=~0)
	{
		if( this->is_sameRef(sub) )
		{	// need an explicit copy
			basestring<T> a(sub);
			this->writeaccess().insert(pos, a, slen);
		}
		else
			this->writeaccess().insert(pos, sub, slen);
		return *this;
	}
	string<T>& insert(size_t pos, const T* c, size_t slen=~0)
	{
		this->writeaccess().insert(pos, c, slen);
		return *this;
	}
	string<T>& insert(size_t pos, const T ch, size_t slen=1)
	{
		this->writeaccess().insert(pos, ch, slen);
		return *this;
	}

	/////////////////////////////////////////////////////////////////
	// replace
	/////////////////////////////////////////////////////////////////
	string<T>& replace(size_t pos, size_t tlen, const stringinterface<T>& t, size_t slen=~0)
	{
		this->writeaccess().replace(pos, tlen, t, slen);
		return *this;
	}
	string<T>& replace(size_t pos, size_t tlen, const substring<T>& sub, size_t slen=~0)
	{
		if( this->is_sameRef(sub) )
		{	// need an explicit copy
			basestring<T> a(sub);
			this->writeaccess().replace(pos, tlen, sub, slen);
		}
		else
			this->writeaccess().replace(pos, tlen, sub, slen);
		return *this;
	}
	string<T>& replace(size_t pos, size_t tlen, const T* c, size_t slen=~0)
	{
		this->writeaccess().replace(pos, tlen, c, slen);
		return *this;
	}
	string<T>& replace(size_t pos, size_t tlen, const T ch, size_t slen=1)
	{
		this->writeaccess().replace(pos, tlen, ch, slen);
		return *this;
	}

	/////////////////////////////////////////////////////////////////
	// standard string functions
	/////////////////////////////////////////////////////////////////
    virtual operator const T*() const	{ return this->readaccess().c_str(); }
	virtual const T* c_str() const		{ return this->readaccess().c_str(); }
	virtual size_t length()	const		{ return this->readaccess().length(); }
	virtual const T* begin() const		{ return this->readaccess().begin(); }
	virtual const T* end() const		{ return this->readaccess().end(); }
	virtual const T* final() const		{ return this->readaccess().final(); }


	void clear()						{ this->writeaccess().clear(); }
	void empty()						{ this->writeaccess().clear(); }
	void clear(size_t inx, size_t len)	{ this->writeaccess().clear(inx, len); }
	bool is_empty() const
	{	
		return ( NULL==this->cCountObj || 0==this->readaccess().length() );
	}
	void truncate(size_t sz)			{ this->writeaccess().truncate(sz); }
	void truncate(size_t ix, size_t sz)	{ this->writeaccess().truncate(ix, sz); }
	bool strip(size_t sz)				{ return this->writeaccess().strip(sz); }
	bool checksize(size_t sz)			{ return this->writeaccess().checksize(sz); }

	/////////////////////////////////////////////////////////////////
	// Array access
	// did not find a way to force the compiler to use the readonly [] operator
	// in any case where it's not written to the object; instead they always
	// use the nonconst operator exept the whole object is const,
	// so we actually do some detour and return a substring which will either 
	// just return the char on readaccess or call the copy-on-write when writing
	// unfortunately the cost of this detour is somewhat higher
	/////////////////////////////////////////////////////////////////
	substring<T>operator[](size_t inx)
	{
		if( inx >= this->length() )
			return substring<T>(this, this->length()-1, 1);
		else
			return substring<T>(this, inx, 1);
	}
	substring<T>operator[](int inx)
	{
		if( inx<=0 )
			return substring<T>(this, 0, 0);
		else if( (size_t)inx >= this->length() )
			return substring<T>(this, this->length()-1, 1);
		else
			return substring<T>(this, inx, 1);
	}
	const T& operator[](size_t inx) const	{ return this->readaccess()[inx]; }
	const T& operator[](int inx) const		{ return this->readaccess()[inx]; }

	/////////////////////////////////////////////////////////////////
	// returns a string to access the string bound to all pointers
	// instead of making it copy on write
	globalstring<T> global()	{ return *this; }

	/////////////////////////////////////////////////////////////////
	// the sub-string operator
	/////////////////////////////////////////////////////////////////
	substring<T> operator()(size_t inx, size_t len=1) const
	{
		if( inx < this->length() )
		{	
			if( inx+len > this->length() )
				len = this->length()-inx;
			return substring<T>(this, inx, len);
		}
		return substring<T>(this, 0, 0);
	}
	string<T> substr(size_t inx, size_t len=1) const
	{
		if( inx < this->length() )
		{
			if( inx+len > this->length() )
				len = this->length()-inx;
			return string<T>(this->c_str()+inx, len);
		}
		return string<T>();
	}

	/////////////////////////////////////////////////////////////////
	// comparisons
	/////////////////////////////////////////////////////////////////
	virtual int compareTo(const string<T> &s) const 
	{	// first check if the pointers are identical
		if( cCountObj != s.cCountObj )
			return this->readaccess().compareTo( s.readaccess() );
		return 0;
	}
	virtual int compareTo(const stringinterface<T>& s) const
	{	
		return this->readaccess().compareTo( s );
	}
	virtual int compareTo(const T *c) const
	{	
		return this->readaccess().compareTo( c );
	}
	virtual int compareTo(const T ch) const
	{	
		return this->readaccess().compareTo( ch );
	}
	/////////////////////////////////////////////////////////////////
	// booleans
	/////////////////////////////////////////////////////////////////
	virtual bool operator ==(const string<T> &s)const { return (this->compareTo( s ) == 0); }
	virtual bool operator !=(const string<T> &s)const { return (this->compareTo( s ) != 0); }
	virtual bool operator <=(const string<T> &s)const { return (this->compareTo( s ) <= 0); }
	virtual bool operator >=(const string<T> &s)const { return (this->compareTo( s ) >= 0); }
	virtual bool operator < (const string<T> &s)const { return (this->compareTo( s ) <  0); }
	virtual bool operator > (const string<T> &s)const { return (this->compareTo( s ) >  0); }

	virtual bool operator ==(const stringinterface<T> &s)	const { return (this->compareTo( s ) == 0); }
	virtual bool operator !=(const stringinterface<T> &s)	const { return (this->compareTo( s ) != 0); }
	virtual bool operator <=(const stringinterface<T> &s)	const { return (this->compareTo( s ) <= 0); }
	virtual bool operator >=(const stringinterface<T> &s)	const { return (this->compareTo( s ) >= 0); }
	virtual bool operator < (const stringinterface<T> &s)	const { return (this->compareTo( s ) <  0); }
	virtual bool operator > (const stringinterface<T> &s)	const { return (this->compareTo( s ) >  0); }

	virtual bool operator ==(const T c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const T *c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T *c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T *c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T *c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T *c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T *c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const basestring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const basestring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const basestring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const basestring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const basestring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const basestring<T>& c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const staticstring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const staticstring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const staticstring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const staticstring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const staticstring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const staticstring<T>& c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const conststring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const conststring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const conststring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const conststring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const conststring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const conststring<T>& c) 	const { return (this->compareTo( c ) >  0); }


	//////////////////////////////////////////////////////
	friend int compare(const string<T> &a,const string<T> &b){ return a.compareTo(b); }


	///////////////////////////////////////////////////////////////////////////
	// search functions
	///////////////////////////////////////////////////////////////////////////
	bool			findnext(const string<T>& pattern, size_t &startpos, bool ignorecase=false) const
	{
		return stringinterface<T>::findnext(pattern,startpos,ignorecase);
	}
	vector<size_t>	findall (const string<T>& pattern, bool ignorecase=false) const
	{
		return stringinterface<T>::findnext(pattern,ignorecase);
	}
	bool			findnext(const T* pattern, size_t &startpos, bool ignorecase=false) const
	{
		conststring<T> tmp(pattern);
		return stringinterface<T>::findnext(tmp,startpos,ignorecase);
	}
	vector<size_t>	findall (const T* pattern, bool ignorecase=false) const
	{
		staticstring<T> tmp(pattern);
		return stringinterface<T>::findnext(tmp,ignorecase);
	}

	size_t find_first_of(const T& ch) const	{ return this->readaccess().find_first_of( ch ); }
	size_t find_last_of(const T& ch) const	{ return this->readaccess().find_last_of( ch ); }

	inline vector< string<T> > split(const T& splitter) const
	{
		vector< string<T> > ret;
		const T *ip, *cp;
		ip = cp = this->c_str();
		if(ip)
		{
			while(*ip)
			{
				if( *ip == splitter)
				{
					ret.push( string<T>(cp, ip-cp) );
					cp = ip = ip+1;
				}
				else
					ip++;				
			}
			if(ip!=cp)
				ret.push( string<T>(cp, ip-cp) );
		}		
		return ret;
	}
	inline vector< string<T> > split(const string<T>& splitter) const
	{
		vector< string<T> > ret;
		const T *ip, *cp;
		ip = cp = this->c_str();
		if(ip)
		{
			while(*ip)
			{
				if( splitter.find_first_of(*ip) != string<>::npos )
				{
					ret.push( string<T>(cp, ip-cp) );
					cp = ip = ip+1;
				}
				else
					ip++;				
			}
			if(ip!=cp)
				ret.push( string<T>(cp, ip-cp) );
		}		
		return ret;
	}


	/////////////////////////////////////////////////////////////////
	// change case
	/////////////////////////////////////////////////////////////////
	virtual void tolower()
	{	// this function does practically nothing if the string
		// contains no uppercase characters. once an uppercase character
		// is encountered, the string is copied to another buffer for copy-on-write strings
		// and the rest is done as usual.

		// a trick to get a non-const pointer without making
		// the string unique
		T *p, *cp, c;
		p = cp = const_cast<T*>(this->c_str());
		bool u = false;
		if(p)
		{
			while(*p)
			{
				c = locase(*p);
				// if the character went lowercase...
				if (c != *p)
				{
					// if the resulting string r is not unique yet...
					if (!u)
					{	// ... do a writeaccess to copy the buffer 
						// and adjust the pointer p accordingly, this is done only once.
						this->writeaccess();
						p = const_cast<T*>(this->c_str()) + (p-cp);
						u = true;
					}
					*p = c;
				}
				p++;
			}
		}
	}
	virtual void toupper()
	{	// a trick to get a non-const pointer without making
		// the string unique
		T *p, *cp, c;
		p = cp = const_cast<T*>(this->c_str());
		bool u = false;
		if(p)
		{
			while(*p)
			{
				c = upcase(*p);
				// if the character went uppercase...
				if (c != *p)
				{
					// if the resulting string r is not unique yet...
					if (!u)
					{	// ... do a writeaccess to copy the buffer 
						// and adjust the pointer p accordingly, this is done only once.
						this->writeaccess();
						p = const_cast<T*>(this->c_str()) + (p-cp);
						u = true;
					}
					*p = c;
				}
				p++;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	// trim's
	///////////////////////////////////////////////////////////////////////////
	// remove whitespaces from left side
	virtual void ltrim()
	{	// a trick to get a non-const pointer without making
		// the string unique
		const T* p=this->c_str(), *q = p;
		if(p)
		{
			while( *p && stringcheck::isspace(*p) )
				p++;
			if(p!=q)
				writeaccess().clear(0, p-q);
		}
	}
	// remove whitespaces from right side
	virtual void rtrim()
	{	// a trick to get a non-const pointer without making
		// the string unique
		const T* e=this->c_str(), *p = e+this->length()-1, *q=p;
		if(e && p>=e)
		{
			while( p>=e && stringcheck::isspace(*p) )
				p--;
			if(p!=q)
				writeaccess().truncate(1+p-e);
		}
	}
	// remove whitespaces from both sides
	virtual void trim()
	{
		rtrim();
		ltrim();
	}
	// remove whitespaces from both sides, combine or remove speces inside
	virtual void itrim(bool removeall=false)
	{
		this->writeaccess().itrim(removeall);
	}
	size_t hash() const
	{
		return this->readaccess().hash();
	}
};


///////////////////////////////////////////////////////////////////////////////
// globalstring
// practically same as string
// but the smart pointer behaviour does not do copy-on-write
// can be used to globally change all strings connected to the same buffer
// use with care in multithread environment
///////////////////////////////////////////////////////////////////////////////
template<class T=char> class globalstring : public string<T>
{
protected:
	/////////////////////////////////////////////////////////////////
	// virtual access functions to the smart pointer
	/////////////////////////////////////////////////////////////////
	// !!autocount behaviour
	virtual const basestring<T>& readaccess() const
	{
		this->checkpointer();	
		// no need to aquire, is done on reference creation
		return *this->cCountObj;
	}
	virtual       basestring<T>& writeaccess(bool keep=true)
	{
		this->checkpointer();
		// no need to aquire, is done on reference creation
		return *this->cCountObj;
	}

public:
	/////////////////////////////////////////////////////////////////
	// construction/destruction
	/////////////////////////////////////////////////////////////////
	globalstring<T>()											{  }
	const globalstring<T>& operator=(const globalstring<T>& r)
	{
		this->string<T>::operator=(r);
		return *this; 
	}
	const globalstring<T>& operator=(const string<T>& r)
	{
		this->string<T>::operator=(r);
		return *this; 
	}
	virtual ~globalstring<T>()									{  }

	/////////////////////////////////////////////////////////////////
	// type construction
	/////////////////////////////////////////////////////////////////
	globalstring<T>(const globalstring<T>& r) : string<T>(r)	{  }
	globalstring<T>(const string<T>& r) : string<T>(r)			{  }
	globalstring<T>(const substring<T>& t) : string<T>(t)		{  }
	globalstring<T>(const char* t) : string<T>(t)				{  }
	explicit globalstring<T>(char t) : string<T>(t)				{  }
	explicit globalstring<T>(int t) : string<T>(t)				{  }
	explicit globalstring<T>(unsigned int t) : string<T>(t)		{  }
	explicit globalstring<T>(long t) : string<T>(t)				{  }
	explicit globalstring<T>(unsigned long t) : string<T>(t)	{  }
	explicit globalstring<T>(double t) : string<T>(t)			{  }

	/////////////////////////////////////////////////////////////////
	// assignment operators
	/////////////////////////////////////////////////////////////////
	const globalstring<T>& operator =(const stringinterface<T>& t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(const substring<T>& t)
	{
		if( this->is_sameRef(t) )
			this->writeaccess().truncate(t.cPos, t.cLen);
		else
			this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(const char* t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(char t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(int t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(unsigned int t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	const globalstring<T>& operator =(double t)
	{
		this->writeaccess(false).assign(t);
		return *this;
	}
	/////////////////////////////////////////////////////////////////
};


///////////////////////////////////////////////////////////////////////////////
// substring
// result of string::operator()
// can only be created from inside a string and not stored, 
// so it is safe to just contain and work on a pointer to the calling string
///////////////////////////////////////////////////////////////////////////////
template <class T=char> class substring : public stringinterface<T>
{
	friend class string<T>;
	string<T>* cString;
	size_t cPos;
	size_t cLen;

// my gnu ver. 3.4 don't like private constructors
// looks like some gcc really insists on having this members public
// even if it's not used and just there to block the default generated copy constructor
#ifdef __GNUC__
public:
#else
private:
#endif
	substring<T>(const substring<T>&);
private:
	substring<T>(const string<T>* str, size_t pos, size_t len)
		: cString(const_cast<string<T>*>(str)), cPos(pos), cLen(len)
	{ }


public:
	~substring<T>()	{}

	const size_t getRefCount() const
	{
		return (this->cString) ? this->cString->getRefCount() : 1;
	}

	virtual operator const T() const	{ return (cString) ? this->cString->c_str()[cPos] : 0; }
	virtual operator const T*() const	{ return (cString) ? this->cString->c_str()+cPos  : this->getStringConstant(0); }
	virtual const T* c_str() const		{ return (cString) ? this->cString->c_str()+cPos  : this->getStringConstant(0); }
	virtual size_t length() const		{ return cLen; }
	virtual const T* begin() const		{ return const_cast<T*>( this->c_str() ); }
	virtual const T* end() const		{ return const_cast<T*>( this->c_str()+cLen ); }
	virtual const T* final() const		{ return const_cast<T*>( this->c_str()+cLen ); }

	/////////////////////////////////////////////////////////////////
	// Different Assignments to a substring
	/////////////////////////////////////////////////////////////////
	const substring<T>& operator=(const string<T>& str)
	{	
		if(cString)
		{
			if( this->cString->is_sameRef(str) )
			{	// handle the recursive assignment
				basestring<T> x(str);	// make a copy
				this->cString->writeaccess().replace(cPos, cLen, x.c_str(), x.length() );
			}
			else
				this->cString->writeaccess().replace(cPos, cLen, str.c_str(), str.length() );
		}
		return *this;
	}
	const substring<T>& operator=(const substring<T>& sub)
	{
		if(cString)
		{
			if( this->cString->is_sameRef(sub) )
			{	// handle the recursive assignment
				basestring<T> x(sub);	// make a copy
				this->cString->writeaccess().replace(cPos, cLen, x.c_str()+sub.cPos, sub.cLen);
			}
			else
				this->cString->writeaccess().replace(cPos, cLen, sub.c_str()+sub.cPos, sub.cLen);
		}
		return *this;
	}
	const substring<T>& operator=(const char* c)
	{	// we assume the given const char in not taken from the string itself
		if(cString)
		{
			size_t sz = (c) ? strlen(c) : 0;
			if(sz) this->cString->writeaccess().replace(cPos, cLen, c, sz);
		}
		return *this;
	}
	const substring<T>& operator=(const char ch)
	{	// replece the selected string potion with one character
		if(ch && cString) this->cString->writeaccess().replace(cPos, cLen, &ch, 1);
		return *this;
	}

	string<T> operator +(const string<T> &s)
	{
		return string<T>(*this) << s;
	}
	string<T> operator +(const stringinterface<T> &t)
	{
		return string<T>(*this) << t;
	}
	string<T> operator +(const substring<T> &sub)
	{
		return string<T>(*this) << sub;
	}
	string<T> operator +(const char* c)
	{
		return string<T>(*this) << c;
	}
	string<T> operator +(const char ch)
	{
		return string<T>(*this) << ch;
	}

	/////////////////////////////////////////////////////////////////
	// compares
	/////////////////////////////////////////////////////////////////
	virtual int compareTo(const substring<T> &sub) const
	{
		if(this->cLen!=sub.cLen)
			return this->cLen-sub.cLen;
		int ret = memcmp(this->c_str(), sub.c_str(), cLen*sizeof(T));
		return ret;
	}
	virtual int compareTo(const string<T> &s) const
	{	// cannot compare up to the terminator because Substrings do not have one
		int ret = memcmp(this->c_str(), s.c_str(), this->cLen*sizeof(T));
		if(ret == 0 ) return (0 - s.c_str()[cLen]);
		return ret;
	}
	virtual int compareTo(const stringinterface<T>& s) const
	{	// cannot compare up to the terminator because Substrings do not have one
		int ret = memcmp(this->c_str(), s.c_str(), cLen*sizeof(T));
		if(ret == 0 ) return (0-s[cLen]);
		return ret;
	}
	virtual int compareTo(const char *c) const
	{	// cannot compare up to the terminator because Substrings do not have one
		int ret = memcmp(this->c_str(), c, cLen*sizeof(T));
		if(ret == 0 ) return (0-c[cLen]);
		return ret;
	}
	virtual int compareTo(const char ch) const
	{
		if(cLen==1)
			return this->c_str()[0] - ch;
		else
			return this->c_str()[1];
	}
	/////////////////////////////////////////////////////////////////
	// change case
	/////////////////////////////////////////////////////////////////
	virtual bool operator ==(const substring<T> &s)	const { return (compareTo( s ) == 0); }
	virtual bool operator !=(const substring<T> &s)	const { return (compareTo( s ) != 0); }
	virtual bool operator <=(const substring<T> &s)	const { return (compareTo( s ) <= 0); }
	virtual bool operator >=(const substring<T> &s)	const { return (compareTo( s ) >= 0); }
	virtual bool operator < (const substring<T> &s)	const { return (compareTo( s ) <  0); }
	virtual bool operator > (const substring<T> &s)	const { return (compareTo( s ) >  0); }

	virtual bool operator ==(const string<T> &s)	const { return (compareTo( s ) == 0); }
	virtual bool operator !=(const string<T> &s)	const { return (compareTo( s ) != 0); }
	virtual bool operator <=(const string<T> &s)	const { return (compareTo( s ) <= 0); }
	virtual bool operator >=(const string<T> &s)	const { return (compareTo( s ) >= 0); }
	virtual bool operator < (const string<T> &s)	const { return (compareTo( s ) <  0); }
	virtual bool operator > (const string<T> &s)	const { return (compareTo( s ) >  0); }

	virtual bool operator ==(const stringinterface<T> &s)	const { return (this->compareTo( s ) == 0); }
	virtual bool operator !=(const stringinterface<T> &s)	const { return (this->compareTo( s ) != 0); }
	virtual bool operator <=(const stringinterface<T> &s)	const { return (this->compareTo( s ) <= 0); }
	virtual bool operator >=(const stringinterface<T> &s)	const { return (this->compareTo( s ) >= 0); }
	virtual bool operator < (const stringinterface<T> &s)	const { return (this->compareTo( s ) <  0); }
	virtual bool operator > (const stringinterface<T> &s)	const { return (this->compareTo( s ) >  0); }

	virtual bool operator ==(const T c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const T *c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const T *c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const T *c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const T *c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const T *c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const T *c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const basestring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const basestring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const basestring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const basestring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const basestring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const basestring<T>& c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const staticstring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const staticstring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const staticstring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const staticstring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const staticstring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const staticstring<T>& c) 	const { return (this->compareTo( c ) >  0); }

	virtual bool operator ==(const conststring<T>& c) 	const { return (this->compareTo( c ) == 0); }
	virtual bool operator !=(const conststring<T>& c) 	const { return (this->compareTo( c ) != 0); }
	virtual bool operator <=(const conststring<T>& c) 	const { return (this->compareTo( c ) <= 0); }
	virtual bool operator >=(const conststring<T>& c) 	const { return (this->compareTo( c ) >= 0); }
	virtual bool operator < (const conststring<T>& c) 	const { return (this->compareTo( c ) <  0); }
	virtual bool operator > (const conststring<T>& c) 	const { return (this->compareTo( c ) >  0); }


	/////////////////////////////////////////////////////////////////
	// basic assign
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& assign(const T* c, size_t slen)
	{
		if(cString)
		{
			this->cString->writeaccess().replace(cPos, cLen, c, slen );
		}
		return *this;
	}
	virtual const stringinterface<T>& assign(const T& c, size_t slen)
	{	// replece the selected string potion with one character
		if(c && cString) this->cString->writeaccess().replace(cPos, cLen, &c, 1);
		return *this;
	}

	/////////////////////////////////////////////////////////////////
	// basic append
	/////////////////////////////////////////////////////////////////
	virtual const stringinterface<T>& append(const T* c, size_t slen)
	{
		if(cString)
		{	// append to the substring == insert after + resize
			size_t sz = this->cString->size();
			this->cString->writeaccess().insert(cPos+cLen, c, slen);
			cLen += this->cString->size() - sz;
			//!! TODO change on modified access interface
		}
		return *this;
	}
	virtual const stringinterface<T>& append(const T& c, size_t slen)
	{
		if(cString)
		{	// append to the substring == insert after + resize
			size_t sz = this->cString->size();
			this->cString->writeaccess().insert(cPos+cLen, c, slen);
			cLen += this->cString->size() - sz;
			//!! TODO change on modified access interface
		}
		return *this;
	}


	/////////////////////////////////////////////////////////////////
	// change case
	/////////////////////////////////////////////////////////////////
	virtual void tolower()
	{
		string<T> sel = *this;
		sel.tolower();
		if(*this!=sel)
			*this=sel;

	}
	virtual void toupper()
	{
		string<T> sel = *this;
		sel.toupper();
		if(*this!=sel)
			*this=sel;
	}
	///////////////////////////////////////////////////////////////////////////
	// trim's
	///////////////////////////////////////////////////////////////////////////
	// remove whitespaces from left side
	virtual void ltrim()
	{	
		string<T> sel = *this;
		sel.ltrim();
		if(*this!=sel)
			*this=sel;
	}
	// remove whitespaces from right side
	virtual void rtrim()
	{
		string<T> sel = *this;
		sel.rtrim();
		if(*this!=sel)
			*this=sel;
	}
	// remove whitespaces from both sides
	virtual void trim()
	{
		string<T> sel = *this;
		sel.trim();
		if(*this!=sel)
			*this=sel;
	}
	// remove whitespaces from both sides, combine or remove speces inside
	virtual void itrim(bool removeall=false)
	{
		string<T> sel = *this;
		sel.itrim(removeall);
		if(*this!=sel)
			*this=sel;
	}
};



///////////////////////////////////////////////////////////////////////////////
// dbstring
// class itself is not usable
// it just contains a simple singleton with a list of all stored strings
// so it can be used to retrieve same stringbuffers throughout the application
///////////////////////////////////////////////////////////////////////////////
template<class T=char> class dbstring
{
private:
	// simple Singleton
	static vector< string<T> >& getStringList()
	{
		static vector< string<T> > cStringList;
		return cStringList;
	}
	dbstring()	{}
	~dbstring()	{}
public:	
	static const string<T>& get(const string<T>& str)
	{
		size_t pos;
		vector< string<T> > &list = getStringList();
		QuickSort< string<T> >( const_cast<string<T>*>(list.begin()), list.size() );
		if( !BinarySearch(str, list.begin(), list.size(), 0, pos) )
		{
			list.insert(str,1,pos);
		}
		return list[pos];
	}
	static const string<T>& get(const char* c)
	{
		size_t pos;
		vector< string<T> > &list = getStringList();
		QuickSort< string<T> >( const_cast<string<T>*>(list.begin()), list.size() );
		if( !BinarySearch(c, list.begin(), list.size(), 0, pos) )
		{
			string<T> s(c);
			list.insert(s,1,pos);
		}
		return list[pos];
	}
	static const string<T>& get(const stringinterface<T>& si)
	{
		size_t pos;
		vector< string<T> > &list = getStringList();
		QuickSort< string<T> >( const_cast<string<T>*>(list.begin()), list.size() );
		if( !BinarySearch(si, list.begin(), list.size(), 0, pos) )
		{
			string<T> s(si);
			list.insert(s,1,pos);
			return list[pos];
		}
		return list[pos];
	}
};


///////////////////////////////////////////////////////////////////////////////
// add operators 
// for left hand side operators, only implement these, 
// the others would confuse standard operators
///////////////////////////////////////////////////////////////////////////////
template<class T> string<T> operator +(const T* t, const stringinterface<T>& str)
{
	return ( string<T>(t) << str );
}
template<class T> string<T> operator +(const T t, const stringinterface<T>& str)
{
	return ( string<T>(t) << str );
}
template<class T> string<T> operator +(const stringinterface<T>& t, const string<T>& str)
{
	return ((string<T>(t)) << str );
}
template<class T> string<T> operator +(const T* t, const string<T>& str)
{
	return ((string<T>(t)) << str );
}
template<class T> string<T> operator +(const T t, const string<T>& str)
{
	return ((string<T>(t)) << str );
}



///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
template<class T> inline void assign(string<T>& s, const char* buf, int len)
{
	s.assign(buf, len);
}
template<class T> inline void clear(string<T>& s)
{
	s.clear();
}
template<class T> inline bool isempty(const stringinterface<T>& s)
{
	return s.length() == 0;
}
template<class T> inline int  pos(const string<T>& s1, const string<T>& s)
{
	return pos(s1.c_str(), s);
}

template<class T> inline string<T> lowercase(const T* p) 
{
    return string<T>().assign_tolower(p);
}

template<class T> inline string<T> lowercase(const string<T>& s)
{
	string<T> r = s;
	r.tolower();
	return r;
}

template<class T> inline string<T> uppercase(const T* p) 
{
    return string<T>().assign_toupper(p);
}

template<class T> inline string<T> uppercase(const string<T>& s)
{
	string<T> r = s;
	r.toupper();
	return r;
}


template<class T> inline string<T> dup(const string<T>& s)
{    // dup() only reads the data pointer so it is thread-safe
    return string<T>(s);
}

template<class T> inline bool contains(const T* s1, size_t s1len, const string<T>& s, size_t at)
{
	return (s1len >= 0) && (at >= 0) && (at+s1len <= hstrlen(s))
        && (s1len == 0 || memcmp(s.c_str()+at, s1, s1len*sizeof(T)) == 0);
}

template<class T> inline bool contains(const T* s1, const string<T>& s, size_t at)
{
    return contains<T>(s1, hstrlen(s1), s, at);
}

template<class T> inline bool contains(const T s1, const string<T>& s, size_t at)
{
    return (at<hstrlen(s)) && (s.c_str()[at] == s1);
}

template<class T> inline bool contains(const string<T>& s1, const string<T>& s, size_t at)
{
    return contains<T>(s1.c_str(), hstrlen(s1), s, at);
}

template<class T> inline string<T> copy(const string<T>& s, size_t from, size_t cnt)
{
    string<T> t;
    if( hstrlen(s)>0 && from<hstrlen(s)) 
    {
        size_t l = min(cnt, hstrlen(s)-from);
        if( from==0 && l==hstrlen(s) )
			t = s;
		else if(l>0)
			t = t(from, cnt);
	}
	return t;
}

template<class T> inline string<T> copy(const string<T>& s, size_t from)
{
	return copy(s, from, INT_MAX);
}

template<class T> inline void ins(const T* s1, size_t s1len, string<T>& s, size_t at)
{
	s.insert(at, s1, s1len);
}

template<class T> inline void ins(const T* sc, string<T>& s, size_t at)
{
	s.insert(at, sc);
}

template<class T> inline void ins(const T c, string<T>& s, size_t at)
{
	s.insert(at, c);
}

template<class T> inline void ins(const string<T>& s1, string<T>& s, size_t at)
{
	s.insert(at, s);
}

template<class T> inline void del(string<T>& s, size_t from, size_t cnt)
{
	s.clear(from, cnt);
}

template<class T> inline void del(string<T>& s, size_t from)
{
	s.clear(from, INT_MAX);
}

template<class T> inline int pos(const T* sc, const string<T>& s)
{
    const T* t = sstrstr(s.c_str(), sc);
    return (t == NULL ? (-1) : (t - s.c_str()));
}

template<class T> inline int pos(T c, const string<T>& s)
{
    const T* t = strchr(s.c_str(), c);
    return (t == NULL ? (-1) : (t - s.c_str()));
}

template<class T> inline int rpos(T c, const string<T>& s)
{
    const T* t = strrchr(s.c_str(), c);
    return (t == NULL ? (-1) : (t - s.c_str()));
}

template<class T> inline string<T>& fill(string<T>& str, size_t width, T ch)
{
    if( width > 0 )
        str.assign(ch, width);
	else
		str.clear();
    return str;
}

template<class T> inline string<T>& pad(string<T>& s, size_t width, T ch, bool left)
{
	size_t len = hstrlen(s);
	if( width > 0 && len < width )
		s.insert( (left)?0:len, ch, width-len);
	return s;
}

template<class T> inline vector< string<T> > split(const string<T>& s, const T& splitter)
{
	return s.split(splitter);
}
template<class T> inline vector< string<T> > split(const string<T>& s, const T* splitter)
{
	return s.split(splitter);
}
template<class T> inline vector< string<T> > split(const string<T>& s, const string<T>& splitter)
{
	return s.split(splitter);
}





///////////////////////////////////////////////////////////////////////////////
// default number to string conversions
// using string default type
///////////////////////////////////////////////////////////////////////////////
string<> itostring(sint64 value, size_t base, size_t width=0, char padchar=0);
string<> itostring(uint64 value, size_t base, size_t width=0, char padchar=0);
string<> itostring(long value, size_t base, size_t width=0, char padchar=0);
string<> itostring(ulong value, size_t base, size_t width=0, char padchar=0);
string<> itostring(int value, size_t base, size_t width=0, char padchar=0);
string<> itostring(uint value, size_t base, size_t width=0, char padchar=0);
string<> ftostring(double v, int prec, size_t width=0, char padchar=0);

inline string<> tostring(sint64 v)	{ return itostring(sint64(v), 10, 0, ' '); }
inline string<> tostring(uint64 v)	{ return itostring(uint64(v), 10, 0, ' '); }
inline string<> tostring(long v)	{ return itostring(sint64(v), 10, 0, ' '); }
inline string<> tostring(ulong v)	{ return itostring(uint64(v), 10, 0, ' '); }
inline string<> tostring(int v)		{ return itostring(sint64(v), 10, 0, ' '); }
inline string<> tostring(uint v)	{ return itostring(uint64(v), 10, 0, ' '); }
inline string<> tostring(double v)	{ return ftostring(v, 6, 0, ' '); }


///////////////////////////////////////////////////////////////////////////////
// timestamp string
///////////////////////////////////////////////////////////////////////////////
template<class T> string<T> nowstring(const T* fmt, bool utc=true);
template<class T> string<T> dttostring(datetime, const T* fmt);
string<> longtimestring(ulong seconds);


////////////////////////////////////////////////////////////////////////////////
string<> bytestostring(long bytes);


extern string<> nullstring;












/*------------------------------------------------------------------------------

  Author:    Andy Rushton
  Copyright: (c) Andy Rushton, 2004
  License:   BSD License, see ../docs/license.html

  Provides an sprintf-like function acting on STL strings rather than char* so
  that there is no possibility of overflowing the string buffer. The 'd' stands
  for "dynamic" in that the string is a dynamic string whereas a char* buffer
  would be static (in size that is, not static in C terms).

int dprintf (std::string&, const char* format, ...);

   Like sprintf(), formats the message by appending to the std::string (using the +=
   operator) according to the formatting codes in the format string. The return int
   is the number of characters generated by this call, i.e. the increase in the
   length of the std::string.

int vdprintf (std::string& formatted, const char* format, va_list args);

   As above, but using a pre-initialised va_args argument list. Useful for nesting
   printf calls within variable argument functions.

std::string dformat (const char* format, ...);

   Similar to dprintf() above, except the result is formatted into a new std::string
   which is returned by the function. Very useful for inline calls within a textio
   expression.
   e.g.    fout << "Total: " << sformat("%6i",t) << newline;

std::string vdformat (const char* format, va_list);

   As above, but using a pre-initialised va_args argument list. Useful for nesting
   sformat calls within variable argument functions.

The result supports the following "C" format codes:

   % [ flags ] [ field ] [ . precision ] [ modifier ] [ conversion ]

 flags:
   -    - left justified
   +    - print sign for +ve numbers
   ' '  - leading space where + sign would be
   0    - leading zeros to width of field
   #    - alternate format

 field:
   a numeric argument specifying the field width - default = 0
   * means take the next va_arg as the field width - if negative then left justify

 precision:
   a numeric argument the meaning of which depends on the conversion -
   - %s - max characters from a string - default = strlen()
   - %e, %f - decimal places to be displayed - default = 6
   - %g - significant digits to be displayed - default = 6
   - all integer conversions - minimum digits to display - default = 0
   * means take the next va_arg as the field width - if negative then left justify

 modifier:
   h    - short or unsigned short
   l    - long or unsigned long
   L    - long double
   
 conversions:
   d, i - short/int/long as decimal
   u    - short/int/long as unsigned decimal
   o    - short/int/long as unsigned octal - # adds leading 0
   x, X - short/int/long as unsigned hexadecimal - # adds leading 0x
   c    - char
   s    - char*
   f    - double/long double as fixed point
   e, E - double/long double as floating point
   g, G - double/long double as fixed point/floating point depending on value
   p    - void* as unsigned hexadecimal
   %    - literal %
   n    - int* as recipient of length of formatted string so far

------------------------------------------------------------------------------*/
template<typename T> ssize_t dsprintf(string<T>& formatted, const T* format, ...);
template<typename T> ssize_t dvsprintf(string<T>& formatted, const T* format, va_list args);

template<typename T> string<T> dprintf(const T* format, ...);
template<typename T> string<T> dvprintf(const T* format, va_list);







#endif//__BASESTRING_H__
