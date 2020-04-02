/*
 * ForceComponent.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: ccontaxis
 */

#include <ForceComponent.h>
#include <cstring>
#include <cmath>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceComponent::ForceComponent() {
    this->name = "UNDEFINED";

    this->enabled = false;
    this->disabling = false;
    this->maxRateOfChange = 15000.0;
    this->nearZeroValue = 10.0;

    this->reset();
}

ForceComponent::~ForceComponent() {}

bool ForceComponent::isEnabled() { return this->enabled; }
bool ForceComponent::isDisabling() { return this->disabling; }

void ForceComponent::enable() {
    // Enable and set the target to 0N
    spdlog::debug("{}ForceComponent: enable()", this->name);
    this->enabled = true;
    this->disabling = false;
    memset(this->xTarget, 0, sizeof(this->xTarget));
    memset(this->yTarget, 0, sizeof(this->yTarget));
    memset(this->zTarget, 0, sizeof(this->zTarget));
    this->postEnableDisableActions();
}

void ForceComponent::disable() {
    // Start disabling and driving to 0N
    spdlog::debug("{}ForceComponent: disable()", this->name);
    this->enabled = false;
    this->disabling = true;
    memset(this->xTarget, 0, sizeof(this->xTarget));
    memset(this->yTarget, 0, sizeof(this->yTarget));
    memset(this->zTarget, 0, sizeof(this->zTarget));
}

void ForceComponent::postEnableDisableActions() {
    // Publish a state change for the component
}

void ForceComponent::update() {
    if (this->disabling) {
        // If we are disabling we need to keep driving this force component to 0N
        // Once we are near zero we consider our action complete and that the force
        // component is actually disabled
        bool nearZero = true;
        for (int i = 0; i < 156 && nearZero; ++i) {
            if (i < 12) {
                nearZero = nearZero && this->xCurrent[i] < this->nearZeroValue &&
                           this->xCurrent[i] > -this->nearZeroValue;
            }

            if (i < 100) {
                nearZero = nearZero && this->yCurrent[i] < this->nearZeroValue &&
                           this->yCurrent[i] > -this->nearZeroValue;
            }

            nearZero = nearZero && this->zCurrent[i] < this->nearZeroValue &&
                       this->zCurrent[i] > -this->nearZeroValue;
        }
        if (nearZero) {
            spdlog::debug("{}ForceComponent: disabled()", this->name);
            this->disabling = false;
            this->enabled = false;
            memset(this->xCurrent, 0, sizeof(this->xCurrent));
            memset(this->yCurrent, 0, sizeof(this->yCurrent));
            memset(this->zCurrent, 0, sizeof(this->zCurrent));
            this->postEnableDisableActions();
            this->postUpdateActions();
        }
    }
    if (this->enabled || this->disabling) {
        // If this force component is enabled then we need to keep trying
        // to drive this force component to it's target value.
        // To do this we need to find the vector with the largest delta
        // and scale all other vectors based off how long it will take to
        // drive that delta to 0N.
        float largestDelta = 0.0;
        for (int i = 0; i < 156; ++i) {
            if (i < 12) {
                this->xOffset[i] = this->xTarget[i] - this->xCurrent[i];
                if (std::abs(this->xOffset[i]) > largestDelta) {
                    largestDelta = std::abs(this->xOffset[i]);
                }
            }

            if (i < 100) {
                this->yOffset[i] = this->yTarget[i] - this->yCurrent[i];
                if (std::abs(this->yOffset[i]) > largestDelta) {
                    largestDelta = std::abs(this->yOffset[i]);
                }
            }

            this->zOffset[i] = this->zTarget[i] - this->zCurrent[i];
            if (std::abs(this->zOffset[i]) > largestDelta) {
                largestDelta = std::abs(this->zOffset[i]);
            }
        }
        // Determine how many outer loop cycles it will take to drive the
        // largest delta to 0N and use that as a scalar for all other
        // actuator deltas.
        float scalar = largestDelta / this->maxRateOfChange;
        if (scalar > 1) {
            // If it is more than 1 outer loop cycle keep working, we aren't
            // then we need to keep working!
            for (int i = 0; i < 156; ++i) {
                if (i < 12) {
                    this->xOffset[i] /= scalar;
                    this->xCurrent[i] += this->xOffset[i];
                }

                if (i < 100) {
                    this->yOffset[i] /= scalar;
                    this->yCurrent[i] += this->yOffset[i];
                }

                this->zOffset[i] /= scalar;
                this->zCurrent[i] += this->zOffset[i];
            }
        } else {
            // If it is less than 1 outer loop cycle just set current as the target
            // we do this to prevent rounding errors from making it so when we
            // request 100N we don't put 99.998N and claim that is what we where asked
            // to produce.
            for (int i = 0; i < 156; ++i) {
                if (i < 12) {
                    this->xCurrent[i] = this->xTarget[i];
                }

                if (i < 100) {
                    this->yCurrent[i] = this->yTarget[i];
                }

                this->zCurrent[i] = this->zTarget[i];
            }
        }
        this->postUpdateActions();
    }
}

void ForceComponent::reset() {
    this->disabling = false;
    this->enabled = false;
    this->postEnableDisableActions();

    memset(this->xCurrent, 0, sizeof(this->xCurrent));
    memset(this->yCurrent, 0, sizeof(this->yCurrent));
    memset(this->zCurrent, 0, sizeof(this->zCurrent));
    memset(this->xTarget, 0, sizeof(this->xTarget));
    memset(this->yTarget, 0, sizeof(this->yTarget));
    memset(this->zTarget, 0, sizeof(this->zTarget));
    memset(this->xOffset, 0, sizeof(this->xOffset));
    memset(this->yOffset, 0, sizeof(this->yOffset));
    memset(this->zOffset, 0, sizeof(this->zOffset));
    this->postUpdateActions();
}

void ForceComponent::postUpdateActions() {
    // Check for forces that need to be clipped
    // Update SAL
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

