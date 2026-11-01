#include "core.h"
#include <iostream>
void check(bool value, const char *expression, int line)
{
    if (!value)
        throw std::runtime_error("Assertion failed at line " + std::to_string(line) + ": " + expression);
}
#define CHECK(expression) check((expression), #expression, __LINE__)
int main() {
    try {
    CHECK(!Settings{}.hdr); CHECK(!Settings::parse(Json::object()).hdr);
    CHECK(!Settings{}.randomPosition);CHECK(!Settings::parse(Json::object()).randomPosition);
    Settings randomSettings;randomSettings.randomPosition=true;
    CHECK(Settings::parse(randomSettings.json()).randomPosition);
    CHECK(Settings{}.mouseEnabled);CHECK(Settings::parse(Json::object()).mouseEnabled);
    CHECK(!Settings::parse({{"mouseEnabled",false}}).mouseEnabled);
    CHECK(Settings{}.keyboardEnabled);CHECK(!Settings::parse({{"keyboardenabled",false}}).keyboardEnabled);
    CHECK(!Settings::parse({{"enabled",false}}).keyboardEnabled);
    CHECK(Settings::parse({{"keyboardenabled",true},{"enabled",false}}).keyboardEnabled);
    CHECK(Settings{}.json().contains("keyboardenabled")&&!Settings{}.json().contains("enabled"));
    CHECK(Settings::parse({{"pack",999},{"volume",-2}}).pack==7);
    CHECK(Settings::parse({{"volume",-2}}).volume==0);
    CHECK(Settings::parse({{"volume",35}}).volume==35);
    CHECK(Settings::parse({{"volume",.35}}).volume==35);
    CHECK(Settings{}.json()["volume"].is_number_integer()&&Settings{}.json()["volume"]==10);
    CHECK(route(9,0)==Sound::BreakPerfect); CHECK(route(32,0)==Sound::TapEx);
    CHECK(route(13,0)==Sound::BreakCritical); CHECK(route(8,0)==Sound::TapEx);
    CHECK(route(44,0)==Sound::Firework); CHECK(route(37,0)==Sound::Touch);
    CHECK(route(45,0)==Sound::BreakPerfect); CHECK(route(16,0)==Sound::Answer);
    CHECK(mouseSound(0,MouseButton::Left)==Sound::Touch);
    CHECK(mouseSound(0,MouseButton::Right)==Sound::Tap);
    CHECK(mouseSound(0,MouseButton::Middle)==Sound::BreakPerfect);
    CHECK(mouseSound(0,MouseButton::Back)==Sound::Answer);
    CHECK(mouseSound(0,MouseButton::Forward)==Sound::Answer);
    for(int pack=1;pack<=7;++pack)for(auto button:{MouseButton::Left,MouseButton::Right,MouseButton::Middle,MouseButton::Back,MouseButton::Forward})CHECK(mouseSound(pack,button)==route(0,pack));
    for(unsigned k=0;k<256;++k) {CHECK(route(k,2)==Sound::Tap);CHECK(route(k,5)==Sound::Firework);CHECK(route(k,6)==Sound::TapEx);CHECK(route(k,7)==Sound::BreakPerfect);}
    CHECK(usableKeyboardKey('A',0x1E));CHECK(!usableKeyboardKey(0,0x1E));
    CHECK(!usableKeyboardKey(0xFF,0x1E));CHECK(!usableKeyboardKey('A',0xFF));
    CHECK(std::abs(hermite(Json::parse("[[0,1,-2,-2],[1,0,0,0]]"),.5)-.25)<1e-9);
    bool rejected=false;try{safeFile("assets","../secret");}catch(...){rejected=true;}CHECK(rejected);
    CHECK(safeFile("assets","star.png")==fs::path("assets")/"star.png");
    Simulation s; for(int i=0;i<30;++i)s.burst(i*.01);CHECK(s.bursts.size()==6);
    auto center=std::array<double,2>{.5,.5};
    CHECK(s.bursts.back().origin==center);
    auto previous=s.bursts.back().origin;
    for(int i=0;i<24;++i){s.burst(.31+i*.01,true);auto origin=s.bursts.back().origin;
        CHECK(s.bursts[s.bursts.size()-2].origin==previous);
        for(auto v:origin)CHECK(v>=.2&&v<=.8);
        previous=origin;CHECK(s.bursts.size()<=6);}
    s.burst(.6);CHECK(s.bursts.back().origin==center);
    s.expire(3);CHECK(s.bursts.empty());
        std::cout<<"PASS: default HDR off, route table, limits, curve, safe paths, burst bound, random origin and expiry\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
