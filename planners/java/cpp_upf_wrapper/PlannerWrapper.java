import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;

import eu.aiplan4eu.jplanner.*;
import java.util.*;

@Platform(includepath={"../../../upf/cpp/"},
          linkpath = {"../../../upf/cpp/"},
          link = {"upf"},
          include={"<optional>", "<set>", "<string>", "<vector>", "action.hxx", "problem.hxx", "heuristic.hxx"},
          compiler={"cpp17"})
public class PlannerWrapper {
    static { Loader.load(); }

    @Namespace("upf") @NoOffset public static class Action extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public Action(Pointer p) { super(p); }
        /** Native array allocator. Access with {@link Pointer#position(long)}. */
        public Action(long size) { super((Pointer)null); allocateArray(size); }
        private native void allocateArray(long size);
        @Override public Action position(long position) {
            return (Action)super.position(position);
        }
        @Override public Action getPointer(long i) {
            return new Action((Pointer)this).position(position + i);
        }
        public Action() { super((Pointer)null); allocate(); }
        private native void allocate();

        public Action(@StdString BytePointer name,
                      @Const @ByRef StringSet precondition,
                      @Const @ByRef StringSet positive_effect,
                      @Const @ByRef StringSet negative_effect) { super((Pointer)null); allocate(name, precondition, positive_effect, negative_effect); }
        private native void allocate(@StdString BytePointer name,
                                     @Const @ByRef StringSet precondition,
                                     @Const @ByRef StringSet positive_effect,
                                     @Const @ByRef StringSet negative_effect);
        public Action(@StdString String name,
                      @Const @ByRef StringSet precondition,
                      @Const @ByRef StringSet positive_effect,
                      @Const @ByRef StringSet negative_effect) { super((Pointer)null); allocate(name, precondition, positive_effect, negative_effect); }
        private native void allocate(@StdString String name,
                                     @Const @ByRef StringSet precondition,
                                     @Const @ByRef StringSet positive_effect,
                                     @Const @ByRef StringSet negative_effect);

        public native @StdString BytePointer name();

        public native @Const @ByRef StringSet precondition();

        public native @Const @ByRef StringSet positive_effect();

        public native @Const @ByRef StringSet negative_effect();

        public native @Cast("bool") @Name("operator <") boolean lessThan(@Const @ByRef Action oth);
    }

    @Name("std::set<std::string>") public static class StringSet extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public StringSet(Pointer p) { super(p); }
        public StringSet()       { allocate();  }
        private native void allocate();
        public native @Name("operator =") @ByRef StringSet put(@ByRef StringSet x);

        public boolean empty() { return size() == 0; }
        public native long size();

        public native void insert(@StdString BytePointer value);
        public native void erase(@StdString BytePointer value);
        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator ++") @ByRef Iterator increment();
            public native @Name("operator ==") boolean equals(@ByRef Iterator it);
            public native @Name("operator *") @StdString BytePointer get();
        }
    }

    @Name("std::vector<upf::Action>") public static class ActionVector extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public ActionVector(Pointer p) { super(p); }
        public ActionVector(Action value) { this(1); put(0, value); }
        public ActionVector(Action ... array) { this(array.length); put(array); }
        public ActionVector()       { allocate();  }
        public ActionVector(long n) { allocate(n); }
        private native void allocate();
        private native void allocate(@Cast("size_t") long n);
        public native @Name("operator =") @ByRef ActionVector put(@ByRef ActionVector x);

        public boolean empty() { return size() == 0; }
        public native long size();
        public void clear() { resize(0); }
        public native void resize(@Cast("size_t") long n);

        @Index(function = "at") public native @ByRef Action get(@Cast("size_t") long i);
        public native ActionVector put(@Cast("size_t") long i, Action value);

        public native @ByVal Iterator insert(@ByVal Iterator pos, @ByRef Action value);
        public native @ByVal Iterator erase(@ByVal Iterator pos);
        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator ++") @ByRef Iterator increment();
            public native @Name("operator ==") boolean equals(@ByRef Iterator it);
            public native @Name("operator *") @ByRef @Const Action get();
        }

        public Action[] get() {
            Action[] array = new Action[size() < Integer.MAX_VALUE ? (int)size() : Integer.MAX_VALUE];
            for (int i = 0; i < array.length; i++) {
                array[i] = get(i);
            }
            return array;
        }
        @Override public String toString() {
            return java.util.Arrays.toString(get());
        }

        public Action pop_back() {
            long size = size();
            Action value = get(size - 1);
            resize(size - 1);
            return value;
        }
        public ActionVector push_back(Action value) {
            long size = size();
            resize(size + 1);
            return put(size, value);
        }
        public ActionVector put(Action value) {
            if (size() != 1) { resize(1); }
            return put(0, value);
        }
        public ActionVector put(Action ... array) {
            if (size() != array.length) { resize(array.length); }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }
    }


    @Name("std::vector<std::string>") public static class StringVector extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public StringVector(Pointer p) { super(p); }
        public StringVector(BytePointer value) { this(1); put(0, value); }
        public StringVector(BytePointer ... array) { this(array.length); put(array); }
        public StringVector(String value) { this(1); put(0, value); }
        public StringVector(String ... array) { this(array.length); put(array); }
        public StringVector()       { allocate();  }
        public StringVector(long n) { allocate(n); }
        private native void allocate();
        private native void allocate(@Cast("size_t") long n);
        public native @Name("operator =") @ByRef StringVector put(@ByRef StringVector x);

        public boolean empty() { return size() == 0; }
        public native long size();
        public void clear() { resize(0); }
        public native void resize(@Cast("size_t") long n);

        @Index(function = "at") public native @StdString BytePointer get(@Cast("size_t") long i);
        public native StringVector put(@Cast("size_t") long i, BytePointer value);
        @ValueSetter @Index(function = "at") public native StringVector put(@Cast("size_t") long i, @StdString String value);

        public native @ByVal Iterator insert(@ByVal Iterator pos, @StdString BytePointer value);
        public native @ByVal Iterator erase(@ByVal Iterator pos);
        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator ++") @ByRef Iterator increment();
            public native @Name("operator ==") boolean equals(@ByRef Iterator it);
            public native @Name("operator *") @StdString BytePointer get();
        }

        public BytePointer[] get() {
            BytePointer[] array = new BytePointer[size() < Integer.MAX_VALUE ? (int)size() : Integer.MAX_VALUE];
            for (int i = 0; i < array.length; i++) {
                array[i] = get(i);
            }
            return array;
        }
        @Override public String toString() {
            return java.util.Arrays.toString(get());
        }

        public BytePointer pop_back() {
            long size = size();
            BytePointer value = get(size - 1);
            resize(size - 1);
            return value;
        }
        public StringVector push_back(BytePointer value) {
            long size = size();
            resize(size + 1);
            return put(size, value);
        }
        public StringVector put(BytePointer value) {
            if (size() != 1) { resize(1); }
            return put(0, value);
        }
        public StringVector put(BytePointer ... array) {
            if (size() != array.length) { resize(array.length); }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }

        public StringVector push_back(String value) {
            long size = size();
            resize(size + 1);
            return put(size, value);
        }
        public StringVector put(String value) {
            if (size() != 1) { resize(1); }
            return put(0, value);
        }
        public StringVector put(String ... array) {
            if (size() != array.length) { resize(array.length); }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }
    }


    @Namespace("upf") @NoOffset public static class Problem extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public Problem(Pointer p) { super(p); }

        public Problem(@Const @ByRef ActionVector actions,
                       @Const @ByRef StringSet init,
                       @Const @ByRef StringSet goal) { super((Pointer)null); allocate(actions, init, goal); }
        private native void allocate(@Const @ByRef ActionVector actions,
                                     @Const @ByRef StringSet init,
                                     @Const @ByRef StringSet goal);

        public native @Const @ByRef ActionVector actions();

        public native @Const @ByRef StringSet init();

        public native @Const @ByRef StringSet goal();
    }

    public static Set<String> convertStringSet(StringSet set) {
        Set<String> res = new HashSet<>();
        for (StringSet.Iterator it = set.begin(); !it.equals(set.end()); it.increment()) {
            res.add(it.get().getString());
        }
        return res;
    }

    public static eu.aiplan4eu.jplanner.Action convertAction(Action action) {
        return new eu.aiplan4eu.jplanner.Action(action.name().getString(),
                                                convertStringSet(action.precondition()),
                                                convertStringSet(action.positive_effect()),
                                                convertStringSet(action.negative_effect()));
    }

    public static Set<eu.aiplan4eu.jplanner.Action> convertActionVector(ActionVector v) {
        Set<eu.aiplan4eu.jplanner.Action> res = new HashSet<>();
        for (int i=0; i<v.size(); i++) {
            res.add(convertAction(v.get(i)));
        }
        return res;
    }

    public static eu.aiplan4eu.jplanner.Problem convertProblem(Problem problem) {
        return new eu.aiplan4eu.jplanner.Problem(convertActionVector(problem.actions()),
                                                 convertStringSet(problem.init()),
                                                 convertStringSet(problem.goal()));
    }


    public static class Heuristic extends Pointer {
        static { Loader.load(); }
        /** Default native constructor. */
        public Heuristic() { super((Pointer)null); allocate(); }
        /** Native array allocator. Access with {@link Pointer#position(long)}. */
        public Heuristic(long size) { super((Pointer)null); allocateArray(size); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public Heuristic(Pointer p) { super(p); }
        private native void allocate();
        private native void allocateArray(long size);
        @Override public Heuristic position(long position) {
            return (Heuristic)super.position(position);
        }
        @Override public Heuristic getPointer(long i) {
            return new Heuristic((Pointer)this).position(position + i);
        }

        @Virtual public native @Name("operator ()") double apply(@Const @ByRef StringSet s);
    }

    public static class JHeuristic implements eu.aiplan4eu.jplanner.IHeuristic {
        public JHeuristic(Heuristic h) {
            this.h = h;
        }

        public double evaluate(Set<String> state) {
            StringSet set = new StringSet();
            for (String str : state) {
                set.insert(new BytePointer(str));
            }
            return h.apply(set);
        }

        private Heuristic h;
    }

    public static class Solve extends FunctionPointer {
        // Loader.load() and allocate() are required only when explicitly creating an instance
        static { Loader.load(); }
        protected Solve() { allocate(); }
        private native void allocate();

        public @Name("jsolve") @ByVal StringVector call(@ByVal Problem problem) throws Exception {
            Solver solver = new Solver();
            List<String> plan = solver.solve(convertProblem(problem));
            return new StringVector(plan.toArray(new String[0]));
        }

    }

    public static class SolveWithHeuristic extends FunctionPointer {
        // Loader.load() and allocate() are required only when explicitly creating an instance
        static { Loader.load(); }
        protected SolveWithHeuristic() { allocate(); }
        private native void allocate();

        public @Name("jsolve_with_heuristic") @ByVal StringVector call(@ByVal Problem problem, @ByRef Heuristic h) throws Exception {
            JHeuristic heuristic = new JHeuristic(h);
            Solver solver = new Solver(heuristic);
            List<String> plan = solver.solve(convertProblem(problem));
            return new StringVector(plan.toArray(new String[0]));
        }

    }

}
