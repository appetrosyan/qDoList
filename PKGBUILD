pkgname="qdolist"
maintainer="a-p-petrosyan@yandex.ru"
pkgver=0.1.0
pkgrel=1
pkgdesc="A to-do list manager for KDE"
arch=("x86_64")
license=('GPL3')
depends=(qt5-base qt5-declarative kdeclarative)
makedepends=(git)
# provides("${pkgname%-*}")	
source=("git+https://github.com/appetrosyan/qDoList")
sha256sums=('SKIP')

prepare() {
  cd "$srcdir/$pkgname"
}

build() {
  cd "$srcdir/$pkgname"
  qmake $pkgname.pro 
  make 
}

package(){
    cd "$srcdir/$pkgname"
    install -D -m755 $pkgname "$pkgdir/usr/bin/$pkgname"
    install -D -m644 "Icons/qDo.svg" "$pkgdir/usr/share/icons/hicolor/scalable/apps/$pkgname.svg"
}
