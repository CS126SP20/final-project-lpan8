//
// Created by lilyl on 4/17/2020.
//

#include "mylibrary/table.h"
#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <cinder/gl/gl.h>
#include "mylibrary/box2d_utility.h"

using namespace cinder;

namespace myapp {
    Wall::Wall(BodyRef body)
            : Body(body, nullptr, vec2(0, 0))
    {
        body_->SetUserData(this);
    }
    void Wall::handleCollision(Ball *ball, const ci::vec2 &contactPoint ) {

    }

    void Wall::draw() {
        float pointsPerMeter = Box2DUtility::getPointsPerMeter();
        auto pos = pointsPerMeter * body_->GetPosition();
        Rectf wallRect( pos.x - width_, 0.0f, pos.x + width_, app::getWindowHeight() );
        gl::drawSolidRect( wallRect );

    }
    Table::Table(myapp::BodyRef body, cinder::gl::TextureRef texture_, vec2 pos)
        : Body(body, texture_, pos)
        {
            body_->SetUserData(this);
    }
    void Table::draw() {
        cinder::Area area = texture_->getBounds();
        cinder::Area bound = app::getWindowBounds();
        cinder::Area fit = Area::proportionalFit(area, bound, true, false);
        cinder::gl::draw(texture_, fit);
    }
    void Table::SetPockets() {
        // hard-coded numbers of positions of pockets
        auto posX = (app::getWindowWidth() - texture_->getWidth()) / 2;
        float right = (float)app::getWindowWidth() - 100.0f;
        float left = (float)app::getWindowWidth() - 710.0f;
        float top = (float)app::getWindowHeight() - 230.0f;
        float bottom = (float)app::getWindowHeight() - 525.0f;

        pockets_.push_back(vec2(left, bottom)); // bottom left
        pockets_.push_back(vec2(left, top)); // top left
        pockets_.push_back(vec2( right, bottom + 10)); // bottom right
        pockets_.push_back(vec2( right, top)); // top right
        pockets_.push_back(vec2(posX + (texture_->getWidth()/2) - 10, bottom)); // bottom middle
        pockets_.push_back(vec2(posX + (texture_->getWidth()/2) - 10, top)); // top middle
    }
    bool Table::is_pocketed(std::shared_ptr<Ball> b)
    {
        for (auto& p : pockets_)
        {
            float x = Box2DUtility::metersToPoints(b->getPos().x );
            float y = Box2DUtility::metersToPoints(b->getPos().y );
            double d = std::hypot(std::abs(x - p.x), std::abs(y - p.y));
            float thresh = b->getRadius();

            // check is ball's position is its radius distance away from pocket
            if (d <= thresh) // hit the pocket
                return true;
        }
        return false;
    }
    void Table::handleCollision(Ball *ball, const ci::vec2 &contactPoint ) {
    }
}
