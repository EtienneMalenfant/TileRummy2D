#pragma once
#include <game/tile_list.h>

// Stratégie de vérification

class IMeldValidator {
public:
    virtual bool isSet(ITileList* tiles) = 0;
    virtual bool isSequence(ITileList* tiles) = 0;
};

class DefaultValidator : public IMeldValidator {
public:
    DefaultValidator() = default;
    bool isSet(ITileList* tiles) override;
    bool isSequence(ITileList* tiles) override;
};

class ValidatorWithJoker : public IMeldValidator {
private:
    Tile* getFirstNonJoker(ITileList* tiles, int& tileIndex);    
public:
    ValidatorWithJoker() = default;
    bool isSet(ITileList* tiles) override;
    bool isSequence(ITileList* tiles) override;
};

// Fabrique de IMeldValidator

class IMeldValidatorFactory {
public:
    virtual IMeldValidator* getDefault() = 0;
    virtual IMeldValidator* getJokerValidator() = 0;
};

class MeldValidatorFactory : public IMeldValidatorFactory {
private:
    IMeldValidator* _defaultValidator;
    IMeldValidator* _jokerValidator;
public:
    MeldValidatorFactory();
    ~MeldValidatorFactory();
    IMeldValidator* getDefault() override;
    IMeldValidator* getJokerValidator() override;
};