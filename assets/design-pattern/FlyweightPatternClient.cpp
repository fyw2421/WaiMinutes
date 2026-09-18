//============================
//FlyweightPatternClient.cpp
//============================
#include "FlyweightPattern.h"

int main(){
    CWebsiteFactory *pWebsiteFactory = new CWebsiteFactory();

    CWebsite *pShowWebsite = pWebsiteFactory->getWebsiteCategory("Product Show");
    pShowWebsite->use(CUser("Cai"));

    pShowWebsite = pWebsiteFactory->getWebsiteCategory("Product Show");
    pShowWebsite->use(CUser("BigBird"));

    delete pShowWebsite;

    CWebsite *pUnshareWebsite = new UnShareWebsite("Test");
    pUnshareWebsite->use(CUser("QA"));
    delete pUnshareWebsite;

    return 1;
}
