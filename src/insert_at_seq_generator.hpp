#pragma once

#include "i_generator.hpp"
#include "ordinal.hpp"
#include <stdexcept>
#include <cstddef>

template <typename T>
class InsertSeqAtGenerator : public IGenerator<T> {
private:
    IGenerator<T>* base_gen_;
    IGenerator<T>* inserted_gen_;

    Ordinal insert_index_;
    Ordinal total_length_;
    Ordinal I_plus_Li_;

    enum class State {
        PRE_INSERT,
        INSERTING,
        POST_INSERT
    };

    mutable State state_;
    Ordinal current_index_;

    static Ordinal subtract_left(const Ordinal& X, const Ordinal& A) {
        if (X.get_omega_count() == A.get_omega_count()) {
            return Ordinal(0, X.get_finite_part() - A.get_finite_part());
        }
        else {
            return Ordinal(X.get_omega_count() - A.get_omega_count(), X.get_finite_part());
        }
    }

public:
    InsertSeqAtGenerator(IGenerator<T>* base, IGenerator<T>* inserted, const Ordinal& insert_idx)
        : base_gen_(base ? base->clone() : nullptr),
        inserted_gen_(inserted ? inserted->clone() : nullptr),
        insert_index_(insert_idx),
        state_(State::PRE_INSERT),
        current_index_(0)
    {
        if (!base_gen_ || !inserted_gen_) {
            throw std::invalid_argument("InsertSeqAtGenerator: generators cannot be nullptr");
        }

        Ordinal L_b = base_gen_->length();
        Ordinal L_i = inserted_gen_->length();

        if (insert_index_ > L_b) {
            throw std::out_of_range("InsertSeqAtGenerator: insert index is greater than base length");
        }

        Ordinal tail_length = subtract_left(L_b, insert_index_);
        I_plus_Li_ = insert_index_ + L_i;
        total_length_ = I_plus_Li_ + tail_length;
    }

    InsertSeqAtGenerator(const InsertSeqAtGenerator& other)
        : base_gen_(other.base_gen_->clone()),
        inserted_gen_(other.inserted_gen_->clone()),
        insert_index_(other.insert_index_),
        total_length_(other.total_length_),
        I_plus_Li_(other.I_plus_Li_),
        state_(other.state_),
        current_index_(other.current_index_) {
    }

    InsertSeqAtGenerator& operator=(const InsertSeqAtGenerator& other) {
        if (this != &other) {
            delete base_gen_;
            delete inserted_gen_;

            base_gen_ = other.base_gen_->clone();
            inserted_gen_ = other.inserted_gen_->clone();
            insert_index_ = other.insert_index_;
            total_length_ = other.total_length_;
            I_plus_Li_ = other.I_plus_Li_;
            state_ = other.state_;
            current_index_ = other.current_index_;
        }
        return *this;
    }

    ~InsertSeqAtGenerator() {
        delete base_gen_;
        delete inserted_gen_;
    }

    Ordinal length() const override {
        return total_length_;
    }

    bool has_next() const override {
        while (true) {
            if (state_ == State::PRE_INSERT) {
                if (current_index_ < insert_index_ && base_gen_->has_next()) {
                    return true;
                }
                state_ = State::INSERTING;
            }
            else if (state_ == State::INSERTING) {
                if (inserted_gen_->has_next()) {
                    return true;
                }
                state_ = State::POST_INSERT;
            }
            else if (state_ == State::POST_INSERT) {
                return base_gen_->has_next();
            }
        }
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("InsertSeqAtGenerator: no more elements");
        }

        if (state_ == State::PRE_INSERT) {
            T val = base_gen_->get_next();
            current_index_ = current_index_ + Ordinal(1);
            return val;
        }
        else if (state_ == State::INSERTING) {
            return inserted_gen_->get_next();
        }
        else {
            return base_gen_->get_next();
        }
    }

    T get_by_ordinal(const Ordinal& index) const override {
        if (index >= total_length_) {
            throw std::out_of_range("InsertSeqAtGenerator: index out of bounds");
        }

        if (index < insert_index_) {
            return base_gen_->get_by_ordinal(index);
        }
        else if (index < I_plus_Li_) {
            Ordinal relative_inserted_idx = subtract_left(index, insert_index_);
            return inserted_gen_->get_by_ordinal(relative_inserted_idx);
        }
        else {
            Ordinal tail_offset = subtract_left(index, I_plus_Li_);
            Ordinal original_base_idx = insert_index_ + tail_offset;
            return base_gen_->get_by_ordinal(original_base_idx);
        }
    }

    IGenerator<T>* clone() const override {
        return new InsertSeqAtGenerator<T>(*this);
    }
};