#pragma once

#include <stdexcept>
#include <string>
#include "i_generator.hpp"
#include "ordinal.hpp"

template <class T>
class FilterGenerator : public IGenerator<T> {
private:
    IGenerator<T>* source_gen_;
    bool (*predicate_)(const T&);

    Ordinal exact_length_;
    mutable T cached_value_;
    mutable bool has_cached_;
    mutable bool source_exhausted_;
    static constexpr std::size_t MAX_STEPS = 1000000;

    void fetch_next() const {
        if (has_cached_ || source_exhausted_) return;

        std::size_t steps = 0;
        while (source_gen_->has_next()) {
            if (steps++ >= MAX_STEPS) {
                throw std::runtime_error(
                    "FilterGenerator: Infinite loop protection triggered. "
                    "Checked " + std::to_string(MAX_STEPS) + " elements without a match."
                );
            }

            T val = source_gen_->get_next();
            if (predicate_(val)) {
                cached_value_ = val;
                has_cached_ = true;
                return;
            }
        }
        source_exhausted_ = true;
    }

public:
    FilterGenerator(const IGenerator<T>& source_gen, bool (*predicate)(const T&))
        : source_gen_(source_gen.clone()),
        predicate_(predicate),
        exact_length_(0, 0),
        has_cached_(false),
        source_exhausted_(false)
    {
        if (predicate == nullptr) {
            throw std::invalid_argument("FilterGenerator: predicate cannot be nullptr");
        }

        if (!source_gen_->length().is_infinite()) {
            std::size_t count = 0;
            IGenerator<T>* temp = source_gen_->clone();
            while (temp->has_next()) {
                if (predicate_(temp->get_next())) {
                    count++;
                }
            }
            delete temp;
            exact_length_ = Ordinal(0, count);
        }
    }

    ~FilterGenerator() {
        delete source_gen_;
    }

    Ordinal length() const override {
        if (source_gen_->length().is_infinite()) {
            throw std::logic_error("FilterGenerator: Cannot determine length of a filtered infinite sequence.");
        }
        return exact_length_;
    }

    bool has_next() const override  {
        fetch_next();
        return has_cached_;
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("FilterGenerator: no more elements match the predicate");
        }
        has_cached_ = false;
        return cached_value_;
    }

    IGenerator<T>* clone() const override {
        return new FilterGenerator<T>(*source_gen_, predicate_);
    }
};