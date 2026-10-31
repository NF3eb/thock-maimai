#include "core.h"
#include <iostream>
void check(bool value) { if(!value)throw std::runtime_error("Assertion failed"); }
int main() {
    check(!Settings{}.hdr); check(!Settings::parse(Json::object()).hdr);
    check(Settings{}.mouseEnabled);check(Settings::parse(Json::object()).mouseEnabled);
    check(!Settings::parse({{"mouseEnabled",false}}).mouseEnabled);
    check(Settings{}.keyboardEnabled);check(!Settings::parse({{"keyboardenabled",false}}).keyboardEnabled);
    check(!Settings::parse({{"enabled",false}}).keyboardEnabled);
    check(Settings::parse({{"keyboardenabled",true},{"enabled",false}}).keyboardEnabled);
    check(Settings{}.json().contains("keyboardenabled")&&!Settings{}.json().contains("enabled"));
    check(Settings::parse({{"pack",999},{"volume",-2}}).pack==7);
    check(Settings::parse({{"volume",-2}}).volume==0);
    check(route(9,0)==Sound::BreakPerfect); check(route(32,0)==Sound::TapEx);
    check(route(13,0)==Sound::BreakCritical); check(route(8,0)==Sound::TapEx);
    check(route(44,0)==Sound::Firework); check(route(37,0)==Sound::Touch);
    check(route(45,0)==Sound::BreakPerfect); check(route(16,0)==Sound::Answer);
    check(mouseSound(0)==Sound::Touch);check(mouseSound(3)==Sound::Touch);
    check(mouseSound(1)==Sound::Answer);check(mouseSound(2)==Sound::Tap);
    check(mouseSound(4)==Sound::BreakCritical);check(mouseSound(5)==Sound::Firework);
    check(mouseSound(6)==Sound::TapEx);check(mouseSound(7)==Sound::BreakPerfect);
    for(unsigned k=0;k<256;++k) {check(route(k,2)==Sound::Tap);check(route(k,5)==Sound::Firework);check(route(k,6)==Sound::TapEx);check(route(k,7)==Sound::BreakPerfect);}
    check(usableKeyboardKey('A',0x1E));check(!usableKeyboardKey(0,0x1E));
    check(!usableKeyboardKey(0xFF,0x1E));check(!usableKeyboardKey('A',0xFF));
    check(std::abs(hermite(Json::parse("[[0,1,-2,-2],[1,0,0,0]]"),.5)-.25)<1e-9);
    bool rejected=false;try{safeFile("assets","../secret");}catch(...){rejected=true;}check(rejected);
    check(safeFile("assets","star.png")==fs::path("assets")/"star.png");
    Simulation s; for(int i=0;i<30;++i)s.burst(i*.01);check(s.bursts.size()==6);
    s.expire(3);check(s.bursts.empty());
    std::cout<<"PASS: default HDR off, route table, limits, curve, safe paths, burst bound and expiry\n";
}
