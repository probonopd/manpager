Name: manpager
Epoch:   1
Version: 1.1.1
Release: 1
Summary: Simple system manual page viewer
License: GPLv3+
Group:   Applications/System
URL:     https://gitlab.com/tychosoft/manpager
Source:  https://www.cherokeesofidaho.org/public/tarballs/%{name}-%{version}.tar.gz
BuildRequires: libqt5-qtbase-devel libqt5-linguist-devel
BuildRequires:  gcc-c++

%description
A simple system manual page viewer offering search.  This is somewhat
more modern version of xman.

%prep
%setup -q

%build
qmake-qt5 QMAKE_CXXFLAGS+="\"%optflags\"" QMAKE_STRIP="%__strip"
%{__make} %{?_smp_mflags}

%install
%{__make} install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%doc README.md LICENSE CHANGELOG
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/metainfo/%{name}.appdata.xml
%{_datadir}/translations/%{name}_*.qm
%{_mandir}/man1/%{name}.1*

