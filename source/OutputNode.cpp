#include "OutputNode.hpp"

namespace AudioProcessor {

    OutputNode::OutputNode() :
    ProcessNodeBase(InputRouter(1),OutputRouter()){

        input_buffer_filled = false;
        input_buffer_id = 0;
    }


    void OutputNode::process(PlayContext& context) {

    }

    NodeUIUpdateResult OutputNode::update_ui() {
        return NodeUIUpdateResult{};
    }

    void OutputNode::input_sample(std::shared_ptr<RealtimeSample> samples) {

        input_buffer_filled = true;
    }
}
