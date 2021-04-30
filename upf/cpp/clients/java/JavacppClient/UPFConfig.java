import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.tools.*;

@Properties(
    value = @Platform(
        includepath = {"./", "../../../"},
        include = {"jupf.hxx", "upf.hxx", "problem.hxx", "action.hxx"},
        linkpath = {"./"},
        link = {"jupf"},
        compiler={"cpp17"}
    ),
    target = "UPF"
)

public class UPFConfig implements InfoMapper {
    public void map(InfoMap infoMap) {
        infoMap.put(new Info("std::set<std::string>").pointerTypes("StringSet").define());
        infoMap.put(new Info("std::vector<std::string>").pointerTypes("StringVector").define());
        infoMap.put(new Info("std::vector<upf::Action>").pointerTypes("ActionVector").define());
        infoMap.put(new Info("std::vector<Action>").pointerTypes("ActionVector"));
        infoMap.put(new Info("std::optional<std::vector<std::string> >").pointerTypes("OptionalStringVector").define());
        infoMap.put(new Info("jupf::Heuristic").virtualize());
        // All std::function<> need to be wrapped in a special way
        infoMap.put(new Info("upf.hxx").linePatterns(".*?std::function.*?").skip());
    }
}
