#include "camera.hpp"

namespace tbyte {
    namespace tools {
        Camera::Camera(ARC_Rect viewSize, ARC_FRect *followTarget, uint32_t followAxis): viewSize(viewSize), followTarget(followTarget){
            offset = { 0.0f, 0.0f };
            scale  = { 0.0f, 0.0f };

            this->followAxis = followAxis;

            pos = { 0.0f, 0.0f };
            update();
        }

        Camera::~Camera(){
        }

        void Camera::update(){
            if(followTarget == nullptr){
                return;
            }

            ARC_FRect scaledViewSize = ARC_Rect_CastToFRect(&viewSize);
            scaledViewSize.w *= scale.x;
            scaledViewSize.h *= scale.y;

            ARC_FRect offsetRect = *followTarget;
            offsetRect.x *= scale.x;
            offsetRect.y *= scale.y;
            offsetRect.w *= scale.x;
            offsetRect.h *= scale.y;
            ARC_FRect_CenterOn(&scaledViewSize, &offsetRect);

            // ARC_FRect windowOffset = { 0, 0, (float)arc::data->windowSize.x, (float)arc::data->windowSize.y };
            // ARC_FRect_CenterOn(&scaledViewSize, &windowOffset);

            if(followAxis & camera::AXIS::X){
                offset.x = pos.x - scaledViewSize.x;
            }

            if(followAxis & camera::AXIS::Y){
                offset.y = pos.y - scaledViewSize.y;
            }
        }

        void Camera::renderScreenCover(){
            ARC_Color coverColor = { 0, 0, 0, 255 };

            if(pos.y > 0){
                ARC_Rect topCover = { 0, 0, arc::data->windowSize.x, (int32_t)pos.y };
                ARC_Rect_RenderFill(&topCover, arc::data->renderer, &coverColor);
            }

            if(pos.x > 0){
                ARC_Rect leftCover = { 0, 0, (int32_t)pos.x, arc::data->windowSize.y };
                ARC_Rect_RenderFill(&leftCover, arc::data->renderer, &coverColor);
            }

            ARC_FRect scaledViewSize = ARC_Rect_CastToFRect(&viewSize);
            scaledViewSize.w *= scale.x;
            scaledViewSize.h *= scale.y;

            if(pos.y + scaledViewSize.h < arc::data->windowSize.y){
                ARC_Rect bottomCover = { 0, (int32_t)(pos.y + scaledViewSize.h), arc::data->windowSize.x, arc::data->windowSize.y - (int32_t)(pos.y + scaledViewSize.h) };
                ARC_Rect_RenderFill(&bottomCover, arc::data->renderer, &coverColor);
            }

            if(pos.x + scaledViewSize.w < arc::data->windowSize.x){
                ARC_Rect rightCover = { (int32_t)(pos.x + scaledViewSize.w), 0, arc::data->windowSize.x - (int32_t)(pos.x + scaledViewSize.w), arc::data->windowSize.y };
                ARC_Rect_RenderFill(&rightCover, arc::data->renderer, &coverColor);
            }
            // ARC_Rect leftCover = { 0, 0, arc::data->windowSize.x, (int32_t)pos.x };
        }

        void Camera::centerOn(ARC_FRect *target){
            followTarget = target;
        }

        void Camera::centerViewOnWindow(){
            ARC_FRect scaledViewSize = ARC_Rect_CastToFRect(&viewSize);
            scaledViewSize.w *= scale.x;
            scaledViewSize.h *= scale.y;

            ARC_FRect windowSize = { 0.0f, 0.0f, (float)arc::data->windowSize.x, (float)arc::data->windowSize.y };
            ARC_FRect_CenterOn(&scaledViewSize, &windowSize);

            pos.x = scaledViewSize.x;
            pos.y = scaledViewSize.y;
        }

        void Camera::setScale(ARC_FPoint scale){
            this->scale = scale;
        }

        ARC_FPoint *Camera::getPos(){
            return &pos;
        }

        ARC_FPoint *Camera::getOffset(){
            return &offset;
        }

        ARC_FPoint *Camera::getScale(){
            return &scale;
        }

        ARC_FRect Camera::getScaledViewSize(){
            ARC_FRect scaledViewSize = ARC_Rect_CastToFRect(&viewSize);
            scaledViewSize.w *= scale.x;
            scaledViewSize.h *= scale.y;

            return scaledViewSize;
        }
    }
}