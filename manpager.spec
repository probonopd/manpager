# Copyright (C) 2017-2019 David Sugar <tychosoft@gmail.com>.
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Name: manpager
Epoch:   1
Version: 1.3.3
Release: 1
Summary: Simple system manual page viewer
License: GPL-3.0+
Group:   Applications/System
URL:     https://www.tychosoft.com
Source:  https://public.tychosoft.com/archive/%{name}-%{version}.tar.gz
BuildRequires: libqt5-qtbase-devel libqt5-linguist-devel
BuildRequires:  gcc-c++

%description
A simple system manual page viewer offering search.  This is somewhat
more modern version of xman.

%prep
%setup -q

%build
qmake-qt5 QMAKE_CXXFLAGS+="%optflags" QMAKE_STRIP=%__strip
%{__make} %{?_smp_mflags}

%install
%{__make} install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%doc README.md LICENSE.md CHANGELOG.md
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/metainfo/%{name}.appdata.xml
%{_mandir}/man1/%{name}.1*

