Name: manpager
Version: 1.0.2
Release: 1
Summary: Simple system manual page viewer
License: GPLv3+
URL:     https://gitlab.com/tychosoft/manpager
Source:  https://pub.cherokeesofidaho.org/tarballs/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: qt5-qtbase-devel qt5-linguist
BuildRequires:  gcc-c++
Group: Applications/System

%description
A simple system manual page viewer offering search.  This is somewhat
more modern version of xman.

%prep
%setup -q

%build
qmake-qt5 QMAKE_CXXFLAGS+="\"%optflags\"" QMAKE_STRIP="/bin/true"
%{__make} %{?_smp_mflags}

%install
%{__make} install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%doc README.md LICENSE CHANGELOG
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/appdata/%{name}.appdata.xml
%{_datadir}/translations/%{name}_*.qm

