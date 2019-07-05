pkgname="qdolist"
_realname="qDoList"
maintainer="a-p-petrosyan@yandex.ru"
pkgver=0.1a
pkgrel=1
pkgdesc="A to-do list manager for KDE"
arch=("x86_64")
license=('GPL3')
depends=(qt5-base qt5-declarative kdeclarative)
makedepends=(git)
# provides("${pkgname%-*}")	
source=("git+https://github.com/appetrosyan/qDoList")
sha256sums=('SKIP')

pkgver() {
    git describe --tags
}
prepare() {
  cd "$srcdir/$_realname"
}

build() {
  cd "$srcdir/$_realname"
  qmake $pkgname.pro 
  make 
}

package(){
    cd "$srcdir/$_realname"
    install -D -m755 $_realname "$pkgdir/usr/bin/$pkgname"
    install -D -m644 "Icons/qDo.svg" "$pkgdir/usr/share/icons/hicolor/scalable/apps/$_realname.svg"
}
