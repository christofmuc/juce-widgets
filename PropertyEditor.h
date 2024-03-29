/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "TypedNamedValue.h"

class PropertyEditor : public juce::Component {
public:
    typedef std::vector<std::shared_ptr<TypedNamedValue>> TProperties;
    explicit PropertyEditor(bool dynamicLayout = false);
    PropertyEditor(TProperties &properties, bool dynamicLayout = false);
    virtual ~PropertyEditor() override;

    virtual void resized() override;

    int getTotalContentHeight();

    void setProperties(TProperties const &props);
    void clear();

    static juce::PropertyComponent *createEditor(std::shared_ptr<TypedNamedValue> property);

    juce::String getLayout();
    void fromLayout(juce::String layout);

private:
    virtual void mouseUp(const juce::MouseEvent &event) override;

    bool dynamicLayout_;
    juce::PropertyPanel propertyPanel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyEditor)
};
