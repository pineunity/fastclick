// -*- c-basic-offset: 4; related-file-name: "../../../elements/standard/portinfo.cc" -*-
#ifndef CLICK_PORTINFO_HH
#define CLICK_PORTINFO_HH
#include <click/element.hh>
#include <click/hashmap.hh>
CLICK_DECLS

/*
=c

PortInfo(NAME PORT/PROTOCOL [NAMES...], ...)

=s information

specifies address information

=io

None

=d

Lets you use mnemonic names for TCP and UDP ports. Each argument has the form
`NAME PORT/PROTOCOL [NAMES...]', which associates the given PORT/PROTOCOL pair
with each of the NAMES. For example, if a configuration contains this PortInfo
element,

   PortInfo(ssh 22, http 80 www);

then other configuration strings can use C<ssh> and C<http> as mnemonics for
the port numbers 22 and 80, respectively.

The mnemonic names introduced by PortInfo elements are local with
respect to compound elements. That is, names created inside a compound
element apply only within that compound element and its subelements. For
example:

   PortInfo(src 10);
   compound :: {
     PortInfo(dst 100);
     ... -> UDPIPEncap(1.0.0.1, src, 2.0.0.1, dst) -> ...  // OK
   };
   ... -> UDPIPEncap(1.0.0.1, src, 2.0.0.1, dst) -> ...
                                         // error: `mazu' undefined

=head1 DEFAULT PORTS

If you do not define a port for a given name, PortInfo will use the default,
if any.  At user level, PortInfo uses the L<getservbyname(3)> function to look
up ports by name.  In the kernel, there are no default ports.

=a

AddressInfo */

class PortInfo : public Element { public:
  
    PortInfo();
    ~PortInfo();
  
    const char *class_name() const	{ return "PortInfo"; }
  
    PortInfo *clone() const		{ return new PortInfo; }
    int configure_phase() const		{ return CONFIGURE_PHASE_FIRST; }
    int configure(Vector<String> &, ErrorHandler *);

    static bool query(const String &, int ip_p, uint16_t &, Element *);

  private:

    enum { INFO_TCP = 0x10000, INFO_UDP = 0x20000 };
  
    HashMap<String, int> _map;

    int add_info(const Vector<String> &, const String &, ErrorHandler *);
    int query(const String &, int, const String &) const;
    static PortInfo *find_element(Element *);
  
};

CLICK_ENDDECLS
#endif
