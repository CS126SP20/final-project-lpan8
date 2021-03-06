//
// Created by lilyl on 4/18/2020.
//

#include "mylibrary/cue.h"
#include "mylibrary/box2d_utility.h"
#include "mylibrary/ball.h"

using namespace cinder;
using namespace cinder::gl;

namespace myapp {

    Cue::Cue(BodyRef body, cinder::gl::TextureRef texture, vec2 pos)
            : Body(body, texture, pos)
    {}
    void Cue::setPosition( const ci::vec2 &pos )
    {
        body_->SetLinearVelocity(b2Vec2(0, 0));
        b2Vec2 p = Box2DUtility::pointsToMeters(b2Vec2(pos.x, pos.y));
        body_->SetTransform( p, 0);
        pos_ = pos;
        hit_ = false;
    }
    float Cue::CalculateAngle(vec2 pos1, vec2 pos2) {
        b2Vec2 p1 = Box2DUtility::pointsToMeters(b2Vec2(pos1.x, pos1.y));
        b2Vec2 p2 = Box2DUtility::pointsToMeters(b2Vec2(pos2.x, pos2.y));
        b2Vec2 dist = p1 - b2Vec2(p2.x, p2.y);
        return atan2f(-dist.x, dist.y);

    }
    void Cue::SetDirection(vec2 pos) {
        direction_ = pos;
        angle_ = CalculateAngle(direction_, pos_);
        auto body = Box2DUtility::pointsToMeters(b2Vec2(pos_.x, pos_.y));
        body_->SetTransform(body, angle_);

    }
    void Cue::IncreasePower() {
        if (power_ <= kMaxPower)
            power_ += kPowerIncrement;
    }

    void Cue::DecreasePower() {
        if (power_ >= kMinPower)
            power_ -= kPowerIncrement;
    }

    void Cue::draw() {

        float conversion = Box2DUtility::getPointsPerMeter();
        vec2 pos = vec2( body_->GetPosition().x, body_->GetPosition().y ) * conversion;
        float t = body_->GetAngle();
        Area destRect = texture_->getBounds();
        gl::ScopedModelMatrix modelScope;
        gl::translate( pos );
        gl::rotate( t );
        gl::draw(texture_, destRect);

    }
    void Cue::ApplyForce() {
        static float dir = -1;
        body_->SetLinearVelocity(b2Vec2(power_*dir*cosf(angle_), power_*dir*sinf(angle_)));
    }

    void Cue::handleCollision(Ball *ball, const ci::vec2 &contactPoint ) {
        auto dir = -1;
        body_->SetLinearVelocity(b2Vec2(power_*cosf(angle_), power_*sinf(angle_)));
        // set the ball's velocity to the same as cue
        ball->GetBody()->SetLinearVelocity(b2Vec2(power_*dir*cosf(angle_), power_*dir*sinf(angle_)));
        // set hit to true after cue collides with ball
        hit_ = true;

    }
}